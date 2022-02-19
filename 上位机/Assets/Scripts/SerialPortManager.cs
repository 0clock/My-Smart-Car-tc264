using UnityEngine;
using System.Collections;
using System.IO.Ports;
using System;
using System.Collections.Generic;
using System.Threading;
using System.Text;

public class SerialPortManager : MonoBehaviour
{
    public string portName = "COM3";//���ںţ�Ӣ�����������COM4��������COM3
    public int baudRate = 115200;//������
    public Parity parity = Parity.None;//Ч��λ
    public int dataBits = 8;//����λ
    public StopBits stopBits = StopBits.One;//ֹͣλ
    public SerialPort sp = null;
    Thread dataReceiveThread;

    public StringBuilder sb = new System.Text.StringBuilder();//���ڴ洢һ�����ȵĽ������ݣ��ַ������������ַ������ݽṹ�ص����ٶȿ�
    public int receiveLength = (4+128*64+4 + 4+1+4 + 4+4+4 + 4+2+4 + 4+1+4) * 2;//ָ��sb���ַ������ȣ�������������ȵ�2��ʱ��� 
    public int baseReceiveLength = (4 + 128 * 64 + 4 + 4 + 1 + 4 + 4 + 4 + 4 + 4 + 2 + 4 + 4 + 1 + 4) * 2;
    public bool flag = false;

    void Start()
    {
        OpenPort();//���ȴ򿪶˿�
        dataReceiveThread = new Thread(new ThreadStart(DataReceiveFunction));
        dataReceiveThread.Start();//������ȡ���ݵ���ѭ���߳�
    }
    public void OpenPort()
    {
        //��������
        sp = new SerialPort(portName, baudRate, parity, dataBits, stopBits);
        sp.ReadTimeout = 400;
        try
        {
            sp.Open();
            Debug.Log("COM4�Ѵ�");
        }
        catch (Exception ex)
        {
            Debug.Log(ex.Message);
        }
    }

    //Ӧ�ùر�ʱ�͹رն˿�
    void OnApplicationQuit()
    {
        ClosePort();
    }

    public void ClosePort()
    {
        try
        {
            sp.Close();
            dataReceiveThread.Abort();
        }
        catch (Exception ex)
        {
            Debug.Log(ex.Message);
        }
    }

    private void Update()
    {
        
    }

    //�������ݵ���ѭ���߳�
    void DataReceiveFunction()
    {       
        int bytes = 0;//�����ж϶�ȡ�Ƿ�ɹ���0��ʧ�ܣ�1�ǳɹ�
        while (true)
        {
            if (sp != null && sp.IsOpen)
            {
                try
                {
                    if (sp.BytesToRead == 0)
                    {
                        continue;
                    }//����������п��Զ�ȡ���ֽ���Ϊ0�������������տɶ�ȡ�ֽ�������Byte����                
                    byte[] buffer = new byte[sp.BytesToRead];//���տɶ�ȡ�ֽ�������Byte����
                    Debug.Log(buffer.Length);//��ʾһ�´�ʱ�ɶ�ȡ�ֽڵĳ���
                    bytes = sp.Read(buffer, 0, buffer.Length);//���ոó��ȵ��ֽ����ݣ��浽Byte����buffer�bytes�����Ƿ��ȡ�ɹ�
                    if (bytes == 0)
                    {
                        continue;
                    }//��ȡʧ�ܣ�������
                    else
                    {
                        string strbytes = Encoding.GetEncoding(37).GetString(buffer);
                        //�����ܵ�����buffer�����ֽ�����תΪ�ַ�����ʹ��37���ַ�����IBM EBCDIC (����-���ô�)����28591���ַ�����ISO-8859-1���������ܱ�֤1�ֽ�һ���ַ�
                        //������������һ���ֽڴ���һ������ʱ�������ַ�����ʹ�ÿ��Ա���ԭʼģ����
                        //��������������ֽڴ���һ�����ݣ�����ʹ��utf-16��
                        //��������ľ���ASCII������ַ�������Ȼ����ֱ��ʹ��ASCII����Encoding.ASCII.GetString(buffer)��
                        sb.Append(strbytes);//��ת������ַ�������sbĩβ
                        if (sb.Length > receiveLength)
                        {
                            sb.Remove(0, sb.Length - receiveLength);//���ڱ�֤sbʼ�ճ���Ϊ�̶���ֵ������ֻ�����µĲ���
                            flag = true;//����ָʾsb�����Ѿ��ﵽ�̶���ֵ
                        }                        
                    }//��ȡ�ɹ�
                }
                catch (Exception ex)
                {
                    if (ex.GetType() != typeof(ThreadAbortException))
                    {
                        Debug.Log("��ȡ������");
                    }
                }
            }
            //Thread.Sleep(1);//1ms����ѭ���������ĵȴ�ʱ����Ա�֤115200������ʱ������Լ��100���ֽڴ���4096�ֽڵĻ�����
        }
    }

    //�����ַ���(��֧��ASCII�ַ�)
    public void WriteData(string dataStr)
    {
        if (sp.IsOpen)
        {
            sp.Write(dataStr);
        }
    }
    //�����ֽ�����
    public void WriteByte(byte[] buffer)
    {
        if (sp.IsOpen)
        {
            sp.Write(buffer, 0, buffer.Length);
        }
    }
}