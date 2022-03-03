using UnityEngine;
using UnityEngine.Experimental.Rendering;
using ComputeShaderUtility;
using System.Text;
using System;
using System.IO;
using UnityEngine.UI;


public class WhiteBoard : MonoBehaviour
{

    // 对所有个体进行计算，计算结果反映为二维图TrailMap（每个元素是一个Float4类型，即Float格式的4元向量）
    const int updateKernel = 0;
    // 对二维图TrailMap加工，输出为二维图diffusedTrailMap
    const int diffuseMapKernel = 1;
    // 将二维图TrailMap（二维图diffusedTrailMap已经复制给它）输出为颜色图displayTexture，它与平面材质贴图已经绑定
    const int colourKernel = 2;

    // 设置Compute Shader对象
    public ComputeShader compute;

    [Header("Agents Settings")]
    // 设置个体的数量
    public int numAgents = 100;

    [Header("Display Settings")]
    // 设置画面宽
    public int width = 188;
    // 设置画面高
    public int height = 40;
    // 设置每帧步长
    public int stepsPerFrame = 1;
    // 设置过滤模式
    public FilterMode filterMode = FilterMode.Point;
    // 设置图形格式
    public GraphicsFormat format = ComputeHelper.defaultGraphicsFormat;

    // 创建渲染纹理
    [SerializeField, HideInInspector] protected RenderTexture trailMap ;
    [SerializeField, HideInInspector] protected RenderTexture diffusedTrailMap;
    [SerializeField, HideInInspector] protected RenderTexture displayTexture;

    // 创建渲染纹理1
    [SerializeField, HideInInspector] protected RenderTexture trailMap1;
    [SerializeField, HideInInspector] protected RenderTexture diffusedTrailMap1;
    [SerializeField, HideInInspector] protected RenderTexture displayTexture1;

    // 创建渲染纹理2
    [SerializeField, HideInInspector] protected RenderTexture trailMap2;
    [SerializeField, HideInInspector] protected RenderTexture diffusedTrailMap2;
    [SerializeField, HideInInspector] protected RenderTexture displayTexture2;
    
    // 创建渲染纹理3
    [SerializeField, HideInInspector] protected RenderTexture trailMap3;
    [SerializeField, HideInInspector] protected RenderTexture diffusedTrailMap3;
    [SerializeField, HideInInspector] protected RenderTexture displayTexture3;

    Picture[] pics = new Picture[188 * 40];//用一个一维数组存储图片数据
    ComputeBuffer picsBuffer;//用于传递图片
    public SerialPortManager serialPortManager;//用于输入外部的串口实例
    string str;
    int index1;
    int index2;

    int UART_Flag_NO_IMAGE = 1;

    float thresholdingValue = 0.25F;

    float cameraAlphaUpOrDown = 40F * 3.1415926F / 2 / 180;//无需校正
    float cameraThetaDown = 27.89191F * 3.1415926F / 180;//需要校正
    float ratioOfMaxDisToHG = 5.915322F;//仅影响显示距离
    float ratioOfPixelToHG = 0.1F;//仅影响分辨率
    public int width_Inverse_Perspective;
    public int height_Inverse_Perspective;
    int ratio = 2;

    float speed_Measured = 0;
    float speed_Output = 0;
    public float speed_Target = 0;

    public float steering_Target = 0;

    public bool writing_Flag = false;
    public bool reading_Flag = false;

    public bool key_Control = true;

    string timeString = DateTime.Now.ToString("yyyy-MM-dd") +" "+ DateTime.Now.ToString("hh-mm-ss");

    float kp, ki, kd;
    float last_error, current_error;

    int repeat_image_cnt = 0;

    int classification_Result=0;
    string[] class_Name_Group = {"左弯", "右弯", "发现右环岛", "右环岛中心", "入右环岛", "出右环岛", "三岔路口", "发现左环岛", "左环岛中心", "入左环岛", "出左环岛", "十字路口","直道"};

    // 启动函数，只会运行一次
    protected virtual void Start()
    {
        // 初始化函数
        Init();
        // 把displayTexture与平面材质绑定
        GameObject.Find("WhiteBoard/Quad").GetComponent<MeshRenderer>().material.mainTexture = displayTexture;
        GameObject.Find("WhiteBoard/Quad1").GetComponent<MeshRenderer>().material.mainTexture = displayTexture1;
        GameObject.Find("WhiteBoard/Quad2").GetComponent<MeshRenderer>().material.mainTexture = displayTexture2;
        GameObject.Find("WhiteBoard/Quad3").GetComponent<MeshRenderer>().material.mainTexture = displayTexture3;
    }


    void Init()
    {
        // 按照宽高、过滤模式和图形格式，初始化贴图纹理
        ComputeHelper.CreateRenderTexture(ref trailMap, width, height, filterMode, format);
        ComputeHelper.CreateRenderTexture(ref diffusedTrailMap, width, height, filterMode, format);
        ComputeHelper.CreateRenderTexture(ref displayTexture, width, height, filterMode, format);

        // 把贴图数据输入Compute Shader中的对应Kernel，并注明在Compute Shader中的变量名，只需要输入一次
        compute.SetTexture(updateKernel, "TrailMap", trailMap);
        compute.SetTexture(diffuseMapKernel, "TrailMap", trailMap);
        compute.SetTexture(diffuseMapKernel, "DiffusedTrailMap", diffusedTrailMap);
        compute.SetTexture(colourKernel, "ColourMap", displayTexture);
        compute.SetTexture(colourKernel, "TrailMap", trailMap);

        // 按照宽高、过滤模式和图形格式，初始化贴图纹理1
        ComputeHelper.CreateRenderTexture(ref trailMap1, width, height, filterMode, format);
        ComputeHelper.CreateRenderTexture(ref diffusedTrailMap1, width, height, filterMode, format);
        ComputeHelper.CreateRenderTexture(ref displayTexture1, width, height, filterMode, format);

        // 把贴图数据1输入Compute Shader中的对应Kernel，并注明在Compute Shader中的变量名，只需要输入一次
        compute.SetTexture(updateKernel, "TrailMap1", trailMap1);
        compute.SetTexture(diffuseMapKernel, "TrailMap1", trailMap1);
        compute.SetTexture(diffuseMapKernel, "DiffusedTrailMap1", diffusedTrailMap1);
        compute.SetTexture(colourKernel, "ColourMap1", displayTexture1);
        compute.SetTexture(colourKernel, "TrailMap1", trailMap1);
   

        width_Inverse_Perspective = (int)Math.Round(2 * ((float)width) / ((float)height*ratio) * Math.Tan(cameraAlphaUpOrDown) / Math.Cos(cameraThetaDown) * ratioOfMaxDisToHG / ratioOfPixelToHG);
        height_Inverse_Perspective = (int)Math.Round(ratioOfMaxDisToHG / ratioOfPixelToHG);
        GameObject.Find("UI/Canvas/Text (3)/InputField").GetComponent<InputField>().text = width_Inverse_Perspective.ToString();
        GameObject.Find("UI/Canvas/Text (3)/InputField (1)").GetComponent<InputField>().text = height_Inverse_Perspective.ToString();


        // 按照宽高、过滤模式和图形格式，初始化贴图纹理2
        ComputeHelper.CreateRenderTexture(ref trailMap2, width_Inverse_Perspective, height_Inverse_Perspective, filterMode, format);
        ComputeHelper.CreateRenderTexture(ref diffusedTrailMap2, width_Inverse_Perspective, height_Inverse_Perspective, filterMode, format);
        ComputeHelper.CreateRenderTexture(ref displayTexture2, width_Inverse_Perspective, height_Inverse_Perspective, filterMode, format);

        // 把贴图数据2输入Compute Shader中的对应Kernel，并注明在Compute Shader中的变量名，只需要输入一次
        compute.SetTexture(updateKernel, "TrailMap2", trailMap2);
        compute.SetTexture(diffuseMapKernel, "TrailMap2", trailMap2);
        compute.SetTexture(diffuseMapKernel, "DiffusedTrailMap2", diffusedTrailMap2);
        compute.SetTexture(colourKernel, "ColourMap2", displayTexture2);
        compute.SetTexture(colourKernel, "TrailMap2", trailMap2);
        
        // 按照宽高、过滤模式和图形格式，初始化贴图纹理3
        ComputeHelper.CreateRenderTexture(ref trailMap3, width_Inverse_Perspective, height_Inverse_Perspective, filterMode, format);
        ComputeHelper.CreateRenderTexture(ref diffusedTrailMap3, width_Inverse_Perspective, height_Inverse_Perspective, filterMode, format);
        ComputeHelper.CreateRenderTexture(ref displayTexture3, width_Inverse_Perspective, height_Inverse_Perspective, filterMode, format);

        // 把贴图数据3输入Compute Shader中的对应Kernel，并注明在Compute Shader中的变量名，只需要输入一次
        compute.SetTexture(updateKernel, "TrailMap3", trailMap3);
        compute.SetTexture(diffuseMapKernel, "TrailMap3", trailMap3);
        compute.SetTexture(diffuseMapKernel, "DiffusedTrailMap3", diffusedTrailMap3);
        compute.SetTexture(colourKernel, "ColourMap3", displayTexture3);
        compute.SetTexture(colourKernel, "TrailMap3", trailMap3);

        // 把各种格式的数据都输入Compute Shader，只需要输入一次
        compute.SetInt("numAgents", numAgents);
        compute.SetInt("width", width);
        compute.SetInt("height", height);
        compute.SetInt("ratio", ratio);

        compute.SetInt("width_Inverse_Perspective", width_Inverse_Perspective);
        compute.SetInt("height_Inverse_Perspective", height_Inverse_Perspective);

        // 如果要输入结构体，必须先用ComputeHelper.CreateStructuredBuffer函数将结构体数据变成Buffer格式，再用compute.SetBuffer输入

        for (int i = 0; i < width * height; i++)
        {
            pics[i].x = i % width;
            pics[i].y = i / width;
            int value = 255;
            pics[i].value = value;
            pics[i].maxvalue = 255;
        }//对图片进行初始化，默认全部是白色
        ComputeHelper.CreateStructuredBuffer(ref picsBuffer, pics);
        compute.SetBuffer(diffuseMapKernel, "pics", picsBuffer);


        GameObject.Find("UI/Canvas/Toggle").GetComponent<Toggle>().isOn = (UART_Flag_NO_IMAGE == 0);


        GameObject.Find("UI/Canvas/Text (2)/Slider").GetComponent<Slider>().value = thresholdingValue;//将Slider的值赋值为二值化阈值
        GameObject.Find("UI/Canvas/Text (2)/InputField").GetComponent<InputField>().text = thresholdingValue.ToString();
        compute.SetFloat("thresholdingValue", thresholdingValue);//将初始二值化阈值输入Compute Shader

        GameObject.Find("UI/Canvas/Text (4)/Slider").GetComponent<Slider>().value = 2 * 180 * cameraAlphaUpOrDown / 3.1415926F;//将Slider的值赋值为cameraAlphaUpOrDown
        GameObject.Find("UI/Canvas/Text (4)/InputField").GetComponent<InputField>().text = (2 * 180 * cameraAlphaUpOrDown / 3.1415926F).ToString();
        compute.SetFloat("cameraAlphaUpOrDown", cameraAlphaUpOrDown);//将初始cameraAlphaUpOrDown输入Compute Shader

        GameObject.Find("UI/Canvas/Text (5)/Slider").GetComponent<Slider>().value = 180 * cameraThetaDown / 3.1415926F;//将Slider的值赋值为cameraThetaDown
        GameObject.Find("UI/Canvas/Text (5)/InputField").GetComponent<InputField>().text = (180 * cameraThetaDown / 3.1415926F).ToString();
        compute.SetFloat("cameraThetaDown", cameraThetaDown);//将初始cameraThetaDown输入Compute Shader

        GameObject.Find("UI/Canvas/Text (6)/Slider").GetComponent<Slider>().value = ratioOfMaxDisToHG;//将Slider的值赋值为ratioOfMaxDisToHG
        GameObject.Find("UI/Canvas/Text (6)/InputField").GetComponent<InputField>().text = ratioOfMaxDisToHG.ToString();
        compute.SetFloat("ratioOfMaxDisToHG", ratioOfMaxDisToHG);//将初始ratioOfMaxDisToHG输入Compute Shader

        GameObject.Find("UI/Canvas/Text (7)/Slider").GetComponent<Slider>().value = ratioOfPixelToHG;//将Slider的值赋值为ratioOfPixelToHG
        GameObject.Find("UI/Canvas/Text (7)/InputField").GetComponent<InputField>().text = ratioOfPixelToHG.ToString();
        compute.SetFloat("ratioOfPixelToHG", ratioOfPixelToHG);//将初始ratioOfPixelToHG输入Compute Shader

        Vector3 scale3;
        scale3.x = 4.0f;
        scale3.y = (float)(height_Inverse_Perspective)/((float)(width_Inverse_Perspective)) * scale3.x;
        scale3.z = 1;
        GameObject.Find("WhiteBoard/Quad2").transform.localScale = scale3;
        GameObject.Find("WhiteBoard/Quad3").transform.localScale = scale3;

        GameObject.Find("UI/Canvas/Text (9)/Slider").GetComponent<Slider>().value = speed_Measured;//将Slider的值赋值为speed_Measured
        GameObject.Find("UI/Canvas/Text (9)/InputField").GetComponent<InputField>().text = speed_Measured.ToString();
        GameObject.Find("UI/Canvas/Text (15)/Slider").GetComponent<Slider>().value = speed_Output;//将Slider的值赋值为speed_Output
        GameObject.Find("UI/Canvas/Text (15)/InputField").GetComponent<InputField>().text = speed_Output.ToString();
        GameObject.Find("UI/Canvas/Text (10)/Slider").GetComponent<Slider>().value = speed_Target;//将Slider的值赋值为speed_Target
        GameObject.Find("UI/Canvas/Text (10)/InputField").GetComponent<InputField>().text = speed_Target.ToString();


        GameObject.Find("UI/Canvas/Text (11)/Slider").GetComponent<Slider>().value = steering_Target;//将Slider的值赋值为steering_Target
        GameObject.Find("UI/Canvas/Text (11)/InputField").GetComponent<InputField>().text = steering_Target.ToString();


        GameObject.Find("UI/Canvas/Toggle (1)").GetComponent<Toggle>().isOn = key_Control;

        GameObject.Find("UI/Canvas/Text (12)/Slider").GetComponent<Slider>().value = kp;//将Slider的值赋值为kp
        GameObject.Find("UI/Canvas/Text (12)/InputField").GetComponent<InputField>().text = kp.ToString();
        GameObject.Find("UI/Canvas/Text (13)/Slider").GetComponent<Slider>().value = ki;//将Slider的值赋值为ki
        GameObject.Find("UI/Canvas/Text (13)/InputField").GetComponent<InputField>().text = ki.ToString();
        GameObject.Find("UI/Canvas/Text (14)/Slider").GetComponent<Slider>().value = kd;//将Slider的值赋值为kd
        GameObject.Find("UI/Canvas/Text (14)/InputField").GetComponent<InputField>().text = kd.ToString();

        GameObject.Find("UI/Canvas/Text (8)/InputField").GetComponent<InputField>().text = class_Name_Group[classification_Result];//将Slider的值赋值为 class_Name_Group[classification_Result]

    }

    // 循环函数，会反复运行
    void FixedUpdate()
    {
        for (int i = 0; i < stepsPerFrame; i++)
        {
            RunSimulation();
        }
    }
    // 循环末尾函数，在每个循环结束后运行一次
    void LateUpdate()
    {
        if (width_Inverse_Perspective * height_Inverse_Perspective < width * height)
        {
            compute.SetInt("width_Choice", 0);
            // 调用colourKernel对应部分，线程分布为( width × height × 1 )，即一共有width × height个线程，坐标按二维分布。功能是将二维图TrailMap（二维图diffusedTrailMap已经复制给它）输出为颜色图displayTexture，它与平面材质贴图已经绑定
            ComputeHelper.Dispatch(compute, width, height, 1, kernelIndex: colourKernel);
        }
        else
        {
            compute.SetInt("width_Choice", 1);
            ComputeHelper.Dispatch(compute, width_Inverse_Perspective, height_Inverse_Perspective, 1, kernelIndex: colourKernel);
        }
    }


    void RunSimulation()
    {
        // 把各种格式的数据都输入Compute Shader，每次循环都要输入一次
        // deltaTime是每帧的实际时间
        compute.SetFloat("deltaTime", Time.fixedDeltaTime);
        // time是实际用时
        compute.SetFloat("time", Time.fixedTime);

        // 用户操作
        UART_Flag_NO_IMAGE = GameObject.Find("UI/Canvas/Toggle").GetComponent<Toggle>().isOn ? 0 : 1;
        if (UART_Flag_NO_IMAGE == 1)
        {
            serialPortManager.receiveLength = serialPortManager.baseReceiveLength - (4 + 188 * 40+ 4) * 2;
        }
        else
        {
            serialPortManager.receiveLength = serialPortManager.baseReceiveLength;
        }

        thresholdingValue = GameObject.Find("UI/Canvas/Text (2)/Slider").GetComponent<Slider>().value;
        GameObject.Find("UI/Canvas/Text (2)/InputField").GetComponent<InputField>().text = thresholdingValue.ToString();
        compute.SetFloat("thresholdingValue", thresholdingValue);

        float cameraAlphaUpOrDown_temp = GameObject.Find("UI/Canvas/Text (4)/Slider").GetComponent<Slider>().value * 3.1415926F / 2 / 180;
        float cameraThetaDown_temp = GameObject.Find("UI/Canvas/Text (5)/Slider").GetComponent<Slider>().value * 3.1415926F / 180;
        float ratioOfMaxDisToHG_temp = GameObject.Find("UI/Canvas/Text (6)/Slider").GetComponent<Slider>().value;
        float ratioOfPixelToHG_temp = GameObject.Find("UI/Canvas/Text (7)/Slider").GetComponent<Slider>().value;

        int width_Inverse_Perspective_temp = (int)Math.Round(2 * ((float)width) / ((float)height*ratio) * Math.Tan(cameraAlphaUpOrDown_temp) / Math.Cos(cameraThetaDown_temp) * ratioOfMaxDisToHG_temp / ratioOfPixelToHG_temp);
        int height_Inverse_Perspective_temp = (int)Math.Round(ratioOfMaxDisToHG_temp / ratioOfPixelToHG_temp);

        if (width_Inverse_Perspective_temp > 1024 || height_Inverse_Perspective_temp > 1024)
        {
            GameObject.Find("UI/Canvas/Text (4)/Slider").GetComponent<Slider>().value = 2 * 180 * cameraAlphaUpOrDown / 3.1415926F;//将Slider的值赋值为cameraAlphaUpOrDown
            GameObject.Find("UI/Canvas/Text (5)/Slider").GetComponent<Slider>().value = 180 * cameraThetaDown / 3.1415926F;//将Slider的值赋值为cameraThetaDown
            GameObject.Find("UI/Canvas/Text (6)/Slider").GetComponent<Slider>().value = ratioOfMaxDisToHG;//将Slider的值赋值为ratioOfMaxDisToHG
            GameObject.Find("UI/Canvas/Text (7)/Slider").GetComponent<Slider>().value = ratioOfPixelToHG;//将Slider的值赋值为ratioOfPixelToHG
        }
        else 
        {
            cameraAlphaUpOrDown = cameraAlphaUpOrDown_temp;
            compute.SetFloat("cameraAlphaUpOrDown", cameraAlphaUpOrDown);
            cameraThetaDown = cameraThetaDown_temp;
            compute.SetFloat("cameraThetaDown", cameraThetaDown);
            ratioOfMaxDisToHG = ratioOfMaxDisToHG_temp;
            compute.SetFloat("ratioOfMaxDisToHG", ratioOfMaxDisToHG);
            ratioOfPixelToHG = ratioOfPixelToHG_temp;
            compute.SetFloat("ratioOfPixelToHG", ratioOfPixelToHG);
        }
        GameObject.Find("UI/Canvas/Text (4)/InputField").GetComponent<InputField>().text = (2 * 180 * cameraAlphaUpOrDown / 3.1415926F).ToString();
        GameObject.Find("UI/Canvas/Text (5)/InputField").GetComponent<InputField>().text = (180 * cameraThetaDown / 3.1415926F).ToString();
        GameObject.Find("UI/Canvas/Text (6)/InputField").GetComponent<InputField>().text = ratioOfMaxDisToHG.ToString();
        GameObject.Find("UI/Canvas/Text (7)/InputField").GetComponent<InputField>().text = ratioOfPixelToHG.ToString();

        width_Inverse_Perspective = (int)Math.Round(2 * ((float)width) / ((float)height*ratio) * Math.Tan(cameraAlphaUpOrDown) / Math.Cos(cameraThetaDown) * ratioOfMaxDisToHG / ratioOfPixelToHG);
        height_Inverse_Perspective = (int)Math.Round(ratioOfMaxDisToHG / ratioOfPixelToHG);

        GameObject.Find("UI/Canvas/Text (3)/InputField").GetComponent<InputField>().text = width_Inverse_Perspective.ToString();
        GameObject.Find("UI/Canvas/Text (3)/InputField (1)").GetComponent<InputField>().text = height_Inverse_Perspective.ToString();

        // 按照宽高修改贴图纹理2
        ComputeHelper.CreateRenderTexture(ref trailMap2, width_Inverse_Perspective, height_Inverse_Perspective, filterMode, format);
        ComputeHelper.CreateRenderTexture(ref diffusedTrailMap2, width_Inverse_Perspective, height_Inverse_Perspective, filterMode, format);
        ComputeHelper.CreateRenderTexture(ref displayTexture2, width_Inverse_Perspective, height_Inverse_Perspective, filterMode, format);
        compute.SetTexture(updateKernel, "TrailMap2", trailMap2);
        compute.SetTexture(diffuseMapKernel, "TrailMap2", trailMap2);
        compute.SetTexture(diffuseMapKernel, "DiffusedTrailMap2", diffusedTrailMap2);
        compute.SetTexture(colourKernel, "ColourMap2", displayTexture2);
        compute.SetTexture(colourKernel, "TrailMap2", trailMap2);
        GameObject.Find("WhiteBoard/Quad2").GetComponent<MeshRenderer>().material.mainTexture = displayTexture2;

        // 按照宽高修改贴图纹理3
        ComputeHelper.CreateRenderTexture(ref trailMap3, width_Inverse_Perspective, height_Inverse_Perspective, filterMode, format);
        ComputeHelper.CreateRenderTexture(ref diffusedTrailMap3, width_Inverse_Perspective, height_Inverse_Perspective, filterMode, format);
        ComputeHelper.CreateRenderTexture(ref displayTexture3, width_Inverse_Perspective, height_Inverse_Perspective, filterMode, format);
        compute.SetTexture(updateKernel, "TrailMap3", trailMap3);
        compute.SetTexture(diffuseMapKernel, "TrailMap3", trailMap3);
        compute.SetTexture(diffuseMapKernel, "DiffusedTrailMap3", diffusedTrailMap3);
        compute.SetTexture(colourKernel, "ColourMap3", displayTexture3);
        compute.SetTexture(colourKernel, "TrailMap3", trailMap3);
        GameObject.Find("WhiteBoard/Quad3").GetComponent<MeshRenderer>().material.mainTexture = displayTexture3;

        compute.SetInt("width_Inverse_Perspective", width_Inverse_Perspective);
        compute.SetInt("height_Inverse_Perspective", height_Inverse_Perspective);

        Vector3 scale3;
        scale3.x = 4.0f;
        scale3.y = (float)(height_Inverse_Perspective) / ((float)(width_Inverse_Perspective)) * scale3.x;
        scale3.z = 1;
        GameObject.Find("WhiteBoard/Quad2").transform.localScale = scale3;
        GameObject.Find("WhiteBoard/Quad3").transform.localScale = scale3;

        speed_Measured = GameObject.Find("UI/Canvas/Text (9)/Slider").GetComponent<Slider>().value;
        GameObject.Find("UI/Canvas/Text (9)/InputField").GetComponent<InputField>().text = speed_Measured.ToString();
        speed_Output = GameObject.Find("UI/Canvas/Text (15)/Slider").GetComponent<Slider>().value;
        GameObject.Find("UI/Canvas/Text (15)/InputField").GetComponent<InputField>().text = speed_Output.ToString();
        speed_Target = GameObject.Find("UI/Canvas/Text (10)/Slider").GetComponent<Slider>().value;
        GameObject.Find("UI/Canvas/Text (10)/InputField").GetComponent<InputField>().text = speed_Target.ToString();


        steering_Target = GameObject.Find("UI/Canvas/Text (11)/Slider").GetComponent<Slider>().value;
        GameObject.Find("UI/Canvas/Text (11)/InputField").GetComponent<InputField>().text = steering_Target.ToString();


        key_Control = GameObject.Find("UI/Canvas/Toggle (1)").GetComponent<Toggle>().isOn;

        kp = GameObject.Find("UI/Canvas/Text (12)/Slider").GetComponent<Slider>().value;
        GameObject.Find("UI/Canvas/Text (12)/InputField").GetComponent<InputField>().text = kp.ToString();
        ki = GameObject.Find("UI/Canvas/Text (13)/Slider").GetComponent<Slider>().value;
        GameObject.Find("UI/Canvas/Text (13)/InputField").GetComponent<InputField>().text = ki.ToString();
        kd = GameObject.Find("UI/Canvas/Text (14)/Slider").GetComponent<Slider>().value;
        GameObject.Find("UI/Canvas/Text (14)/InputField").GetComponent<InputField>().text = kd.ToString();

        GameObject.Find("UI/Canvas/Text (8)/InputField").GetComponent<InputField>().text = class_Name_Group[classification_Result];

        //...


        // 用户发起写入要求
        byte[] byteArray_Send;

        //发送图片是否传输的信号，数据头00-FF-01-01，数据长度1字节，数据尾00-FF-01-02
        byteArray_Send = new byte[] { 0x00, 0xFF, 0x01, 0x01, (byte)UART_Flag_NO_IMAGE, 0x00, 0xFF, 0x01, 0x02 };
        serialPortManager.WriteByte(byteArray_Send);
        Debug.Log(BitConverter.ToString(byteArray_Send));

        if (writing_Flag == true)
        {
     
            //发送二值化阈值，数据头00-FF-02-01，数据长度1字节，数据尾00-FF-02-02
            byteArray_Send = new byte[] {0x00,0xFF,0x02,0x01, (byte)(255*thresholdingValue), 0x00, 0xFF, 0x02, 0x02};
            serialPortManager.WriteByte(byteArray_Send);
            Debug.Log(BitConverter.ToString(byteArray_Send));

            
            //发送逆透视参数，数据头00-FF-03-01，数据长度1字节，数据尾00-FF-03-02
            byteArray_Send = new byte[] { 0x00, 0xFF, 0x03, 0x01, (byte)(Math.Round((cameraAlphaUpOrDown * 2 * 180 / 3.1415926 - 0) / (90 - 0) * 255)),(byte)(Math.Round((cameraThetaDown * 180 / 3.1415926 - 0) / (90 - 0) * 255)),(byte)(Math.Round((ratioOfMaxDisToHG - 0) / (15 - 0) * 255)),(byte)(Math.Round((ratioOfPixelToHG - 0) / (0.1 - 0) * 255)), 0x00, 0xFF, 0x03, 0x02 };
            serialPortManager.WriteByte(byteArray_Send);
            Debug.Log(BitConverter.ToString(byteArray_Send));

            //发送速度参数，数据头00-FF-04-01，数据长度1字节，数据尾00-FF-04-02
            byteArray_Send = new byte[] { 0x00, 0xFF, 0x04, 0x01, (byte)(Math.Round((speed_Target - (-7)) / (7 - (-7)) * 255)), 0x00, 0xFF, 0x04, 0x02 };
            serialPortManager.WriteByte(byteArray_Send);
            Debug.Log(BitConverter.ToString(byteArray_Send));

            //发送舵机参数，数据头00-FF-05-01，数据长度1字节，数据尾00-FF-05-02
            byteArray_Send = new byte[] { 0x00, 0xFF, 0x05, 0x01, (byte)(Math.Round((steering_Target - (-30)) / (30 - (-30)) * 255)), 0x00, 0xFF, 0x05, 0x02 };
            serialPortManager.WriteByte(byteArray_Send);
            Debug.Log(BitConverter.ToString(byteArray_Send));

            //发送PID参数，数据头00-FF-07-01，数据长度6字节，数据尾00-FF-07-02
            byteArray_Send = new byte[] { 0x00, 0xFF, 0x07, 0x01, (byte)(((Int16)(Math.Round(kp*1000)))>>8), (byte)(((Int16)(Math.Round(kp * 1000)))), (byte)(((Int16)(Math.Round(ki * 1000))) >> 8), (byte)(((Int16)(Math.Round(ki * 1000)))), (byte)(((Int16)(Math.Round(kd * 1000))) >> 8), (byte)(((Int16)(Math.Round(kd * 1000)))), 0x00, 0xFF, 0x07, 0x02 };
            serialPortManager.WriteByte(byteArray_Send);
            Debug.Log(BitConverter.ToString(byteArray_Send));

            //...
        }


        //当串行通信收到的信息长度满足等于设置长度时
        if (serialPortManager.flag == true)
        {
            Debug.Log(serialPortManager.sb.Length);//输出字符串长度
            str = serialPortManager.sb.ToString();//转成string

            byte[] byteArray = Encoding.GetEncoding(37).GetBytes(str);//将string解码为Byte数组，依然使用37号字符集
            str = BitConverter.ToString(byteArray);//为了便于查找数据头00-FF-01-01，将Byte数组转为“...-XX-XX-XX-XX-XX-...”的字符串形式

            Debug.Log(str);

            //一直更新，不需要用户发起读取请求
            //接收分类参数，数据头00-FF-08-01，数据长度1字节，数据尾00-FF-08-02
            index1 = str.IndexOf("00-FF-08-01") / 3;//找到第一个数据头
            if (index1 != -1)
            {
                index2 = (str.Substring(index1 * 3, str.Length - 1 - index1 * 3)).IndexOf("00-FF-08-02") / 3;//找到最后一个数据头（也就是第二个数据头） 
                if (index2 == 4 + 1)//检测两个数据头之间是否有正确的字节数目
                {
                    int value = byteArray[index1 + 4];
                    GameObject.Find("UI/Canvas/Text (8)/InputField").GetComponent<InputField>().text = class_Name_Group[value];
                    classification_Result = value;
                }
            }
            //接收图片，数据头00-FF-01-01，数据长度width * height字节，数据尾00-FF-01-02
            index1 = str.IndexOf("00-FF-01-01")/3;//找到第一个数据头
            if (index1 != -1)
            {
                index2 = (str.Substring(index1*3,str.Length-1-index1*3)).IndexOf("00-FF-01-02")/3;//找到最后一个数据头（也就是第二个数据头） 
                if (index2 == 4 + width * height)//检测两个数据头之间是否有正确的字节数目
                {
                    for (int i = 0; i < width * height; i++)
                    {
                        pics[i].x = i % width;
                        pics[i].y = i / width;
                        int value = byteArray[i + index1 + 4];
                        pics[i].value = value;
                        pics[i].maxvalue = 255;                  
                    }//将数据赋给图片一维数组
                    ComputeHelper.CreateStructuredBuffer(ref picsBuffer, pics);
                    compute.SetBuffer(diffuseMapKernel, "pics", picsBuffer);//传入Compute Shader

                    if (repeat_image_cnt==0)
                    {
                        string path = @"D:\CarData\" + timeString + @" Images\";
                        if (!Directory.Exists(path))
                        {
                            Directory.CreateDirectory(path);
                        }
                        FileStream fs = new FileStream(path+ Time.fixedTime.ToString()+class_Name_Group[classification_Result]+".pgm", FileMode.Create);
                        byte[] data1 = Encoding.UTF8.GetBytes("P5\n"+"188 40\n"+"255\n");
                        byte[] data2 = new byte[width * height];
                        for (int i = 0; i < width * height; i++)
                        {
                            data2[i] = byteArray[i + index1 + 4];
                        }
                        fs.Write(data1, 0, data1.Length);
                        fs.Write(data2, 0, data2.Length);
                        fs.Flush();
                        fs.Close();
                    }
                    repeat_image_cnt = repeat_image_cnt + 1;
                    if (repeat_image_cnt == 30)
                    {
                        repeat_image_cnt = 0;
                    }

                }
            }

            //用户发起读取要求时才更新
            //接收二值化阈值，数据头00-FF-02-01，数据长度1字节，数据尾00-FF-02-02
            index1 = str.IndexOf("00-FF-02-01") / 3;//找到第一个数据头
            if (index1 != -1)
            {
                index2 = (str.Substring(index1*3, str.Length - 1 - index1*3)).IndexOf("00-FF-02-02") / 3;//找到最后一个数据头（也就是第二个数据头） 
                if (index2 == 4+1 && reading_Flag == true)//检测两个数据头之间是否有正确的字节数目
                {
                    int value = byteArray[index1 + 4];
                    GameObject.Find("UI/Canvas/Text (2)/Slider").GetComponent<Slider>().value = (float)value / 255;
                    thresholdingValue = (float)value/255;//更新Slider的值，赋值为二值化阈值
                    compute.SetFloat("thresholdingValue", thresholdingValue);//更新Compute Shader的二值化阈值
                }
            }
            //接收逆透视参数，数据头00-FF-03-01，数据长度4字节，数据尾00-FF-03-02
            index1 = str.IndexOf("00-FF-03-01") / 3;//找到第一个数据头
            if (index1 != -1)
            {
                index2 = (str.Substring(index1 * 3, str.Length - 1 - index1 * 3)).IndexOf("00-FF-03-02") / 3;//找到最后一个数据头（也就是第二个数据头） 
                if (index2 == 4 + 4 && reading_Flag == true)//检测两个数据头之间是否有正确的字节数目
                {
                    int value = byteArray[index1 + 4];
                    GameObject.Find("UI/Canvas/Text (4)/Slider").GetComponent<Slider>().value = (float)value / 255 * (90-0)+0;
                    cameraAlphaUpOrDown = ((float)value / 255 * (90 - 0) + 0) * 3.1415926F / 2 / 180;//更新Slider的值，赋值
                    compute.SetFloat("cameraAlphaUpOrDown", cameraAlphaUpOrDown);//更新Compute Shader

                    value = byteArray[index1 + 5];
                    GameObject.Find("UI/Canvas/Text (5)/Slider").GetComponent<Slider>().value = (float)value / 255 * (90-0)+0;
                    cameraThetaDown = ((float)value / 255 * (90 - 0) + 0) * 3.1415926F / 180;//更新Slider的值，赋值
                    compute.SetFloat("cameraThetaDown", cameraThetaDown);//更新Compute Shader

                    value = byteArray[index1 + 6];
                    GameObject.Find("UI/Canvas/Text (6)/Slider").GetComponent<Slider>().value = (float)value / 255 * (15-0)+0;
                    ratioOfMaxDisToHG = (float)value / 255 * (15 - 0) + 0;//更新Slider的值，赋值
                    compute.SetFloat("ratioOfMaxDisToHG", ratioOfMaxDisToHG);//更新Compute Shader

                    value = byteArray[index1 + 7];
                    GameObject.Find("UI/Canvas/Text (7)/Slider").GetComponent<Slider>().value = (float)value / 255 * (0.1F-0)+0;
                    ratioOfPixelToHG = (float)value / 255 * (0.1F - 0) + 0;//更新Slider的值，赋值
                    compute.SetFloat("ratioOfPixelToHG", ratioOfPixelToHG);//更新Compute Shader
                }
            }
            //接收速度参数，数据头00-FF-04-01，数据长度5字节，数据尾00-FF-04-02
            index1 = str.IndexOf("00-FF-04-01") / 3;//找到第一个数据头
            if (index1 != -1)
            {
                index2 = (str.Substring(index1 * 3, str.Length - 1 - index1 * 3)).IndexOf("00-FF-04-02") / 3;//找到最后一个数据头（也就是第二个数据头） 
                if (index2 == 4 + 5)//检测两个数据头之间是否有正确的字节数目
                {
                    int value = byteArray[index1 + 4];
                    GameObject.Find("UI/Canvas/Text (9)/Slider").GetComponent<Slider>().value = (float)value / 256 * (7-(-7))+(-7);
                    speed_Measured = (float)value / 256 * (7 - (-7)) + (-7);//更新Slider的值，赋值

                    value = byteArray[index1 + 5];
                    GameObject.Find("UI/Canvas/Text (15)/Slider").GetComponent<Slider>().value = (float)value / 256 * (7 - (-7)) + (-7);
                    speed_Output = (float)value / 256 * (7 - (-7)) + (-7);//更新Slider的值，赋值

                    value = byteArray[index1 + 7];
                    last_error = (float)value / 256 * (7 - (-7)) + (-7);//更新Slider的值，赋值
                    value = byteArray[index1 + 8];
                    current_error = (float)value / 256 * (7 - (-7)) + (-7);//更新Slider的值，赋值

                    string path = @"D:\CarData\" + timeString + @" Others\";
                    if (!Directory.Exists(path))
                    {
                        Directory.CreateDirectory(path);
                    }
                    FileStream fs = new FileStream(path + "speed_Measured.txt", FileMode.Append);
                    byte[] data = Encoding.UTF8.GetBytes(Time.fixedTime.ToString()+","+ speed_Measured.ToString() + "\r\n");
                    fs.Write(data, 0, data.Length);
                    fs.Flush();
                    fs.Close();

                    fs = new FileStream(path + "speed_Output.txt", FileMode.Append);
                    data = Encoding.UTF8.GetBytes(Time.fixedTime.ToString() + "," + speed_Output.ToString() + "\r\n");
                    fs.Write(data, 0, data.Length);
                    fs.Flush();
                    fs.Close();

                    fs = new FileStream(path + "error.txt", FileMode.Append);
                    data = Encoding.UTF8.GetBytes(Time.fixedTime.ToString() + "," + last_error.ToString() +','+ current_error.ToString() + "\r\n");
                    fs.Write(data, 0, data.Length);
                    fs.Flush();
                    fs.Close();

                    fs = new FileStream(path + "steering_Target.txt", FileMode.Append);
                    data = Encoding.UTF8.GetBytes(Time.fixedTime.ToString() + "," + steering_Target.ToString() + "\r\n");
                    fs.Write(data, 0, data.Length);
                    fs.Flush();
                    fs.Close();

                    if (reading_Flag == true)
                    {
                        value = byteArray[index1 + 6];
                        GameObject.Find("UI/Canvas/Text (10)/Slider").GetComponent<Slider>().value = (float)value / 256 * (7 - (-7)) + (-7);
                        speed_Target = (float)value / 256 * (7 - (-7)) + (-7);//更新Slider的值，赋值
                    }


                }
            }
            //接收舵机参数，数据头00-FF-05-01，数据长度1字节，数据尾00-FF-05-02
            index1 = str.IndexOf("00-FF-05-01") / 3;//找到第一个数据头
            if (index1 != -1)
            {
                index2 = (str.Substring(index1 * 3, str.Length - 1 - index1 * 3)).IndexOf("00-FF-05-02") / 3;//找到最后一个数据头（也就是第二个数据头） 
                if (index2 == 4 + 1 && reading_Flag == true)//检测两个数据头之间是否有正确的字节数目
                {
                    int value = byteArray[index1 + 4];
                    GameObject.Find("UI/Canvas/Text (11)/Slider").GetComponent<Slider>().value = (float)value / 256 * (30 - (-30)) + (-30);
                    steering_Target = (float)value / 256 * (30 - (-30)) + (-30);//更新Slider的值，赋值为二值化阈值
                }
            }
            //接收pid参数，数据头00-FF-07-01，数据长度6字节，数据尾00-FF-07-02
            index1 = str.IndexOf("00-FF-07-01") / 3;//找到第一个数据头
            if (index1 != -1)
            {
                index2 = (str.Substring(index1 * 3, str.Length - 1 - index1 * 3)).IndexOf("00-FF-07-02") / 3;//找到最后一个数据头（也就是第二个数据头） 
                if (index2 == 4 + 6 && reading_Flag == true)//检测两个数据头之间是否有正确的字节数目
                {
                    int value = byteArray[index1 + 4]*256+ byteArray[index1 + 5];
                    if (value > 32767)
                    {
                        value = value - 65536;
                    }
                    GameObject.Find("UI/Canvas/Text (12)/Slider").GetComponent<Slider>().value = value/1000.0f;
                    kp = value/1000.0f;//更新Slider的值，赋值

                    value = byteArray[index1 + 6] * 256 + byteArray[index1 + 7];
                    if (value > 32767)
                    {
                        value = value - 65536;
                    }
                    GameObject.Find("UI/Canvas/Text (13)/Slider").GetComponent<Slider>().value = value / 1000.0f;
                    ki = value / 1000.0f;//更新Slider的值，赋值

                    value = byteArray[index1 + 8] * 256 + byteArray[index1 + 9];
                    if (value > 32767)
                    {
                        value = value - 65536;
                    }
                    GameObject.Find("UI/Canvas/Text (14)/Slider").GetComponent<Slider>().value = value / 1000.0f;
                    kd = value / 1000.0f;//更新Slider的值，赋值

                    string path = @"D:\CarData\" + timeString + @" Others\";
                    if (!Directory.Exists(path))
                    {
                        Directory.CreateDirectory(path);
                    }
                    FileStream fs = new FileStream(path + "PID.txt", FileMode.Append);
                    byte[] data = Encoding.UTF8.GetBytes(Time.fixedTime.ToString() + "," + kp.ToString()+ "," + ki.ToString()+ "," + kd.ToString() + "\r\n");
                    fs.Write(data, 0, data.Length);
                    fs.Flush();
                    fs.Close();

                }
            }
            
            //...

            serialPortManager.flag = false;
        }

        // 每次循环，调用一次Compute Shader进行并行运算
        if (width_Inverse_Perspective*height_Inverse_Perspective < width*height)
        {
            compute.SetInt("width_Choice", 0);
            // 调用diffuseMapKernel对应部分，线程分布为( width × height × 1 )，即一共有width × height个线程，坐标按二维分布。功能是对二维图TrailMap加工，输出为二维图diffusedTrailMap
            ComputeHelper.Dispatch(compute, width, height, 1, kernelIndex: diffuseMapKernel);
        }
        else
        {
            compute.SetInt("width_Choice", 1);
            // 调用diffuseMapKernel对应部分，线程分布为( width × height × 1 )，即一共有width × height个线程，坐标按二维分布。功能是对二维图TrailMap加工，输出为二维图diffusedTrailMap
            ComputeHelper.Dispatch(compute, width_Inverse_Perspective, height_Inverse_Perspective, 1, kernelIndex: diffuseMapKernel);
        }
        
        // 调用updateKernel对应部分，线程分布为( numAgents × 1 × 1 )，即一共有numAgents个线程，坐标按线性分布。功能是完成所有个体进行计算，计算结果反映为二维图TrailMap
        //ComputeHelper.Dispatch(compute, numAgents, 1, 1, kernelIndex: updateKernel);
        

        // 将diffusedTrailMap复制给trailMap
        ComputeHelper.CopyRenderTexture(diffusedTrailMap, trailMap);
        ComputeHelper.CopyRenderTexture(diffusedTrailMap1, trailMap1);
        ComputeHelper.CopyRenderTexture(diffusedTrailMap2, trailMap2); 
        ComputeHelper.CopyRenderTexture(diffusedTrailMap3, trailMap3);
    }
    public struct Picture
    {
        public float x, y;
        public float value;
        public float maxvalue;
    }

    public void Toggle_Reading_Flag()
    {
        reading_Flag = !reading_Flag;
        if (reading_Flag)
        {
            Sprite sprite = Resources.Load("GreenCircle",typeof(Sprite)) as Sprite;
            GameObject.Find("UI/Canvas/Button/Image").GetComponent<Image>().sprite = sprite;
        }
        else
        {
            Sprite sprite = Resources.Load("RedCircle", typeof(Sprite)) as Sprite;
            GameObject.Find("UI/Canvas/Button/Image").GetComponent<Image>().sprite = sprite;
        }     
    }

    public void Toggle_writing_Flag()
    {
        writing_Flag = !writing_Flag;
        if (writing_Flag)
        {
            Sprite sprite = Resources.Load("GreenCircle", typeof(Sprite)) as Sprite;
            GameObject.Find("UI/Canvas/Button1/Image").GetComponent<Image>().sprite = sprite;
        }
        else
        {
            Sprite sprite = Resources.Load("RedCircle", typeof(Sprite)) as Sprite;
            GameObject.Find("UI/Canvas/Button1/Image").GetComponent<Image>().sprite = sprite;
        }
    }


}



