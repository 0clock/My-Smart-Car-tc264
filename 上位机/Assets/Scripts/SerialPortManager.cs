using UnityEngine;
using System.Collections;
using System.IO.Ports;
using System;
using System.Collections.Generic;
using System.Threading;
using System.Text;

public class SerialPortManager : MonoBehaviour
{
    public string portName = "COM3";//串口号，英飞凌调试器是COM4，蓝牙是COM3
    public int baudRate = 115200;//波特率
    public Parity parity = Parity.None;//效验位
    public int dataBits = 8;//数据位
    public StopBits stopBits = StopBits.One;//停止位
    public SerialPort sp = null;
    Thread dataReceiveThread;

    public StringBuilder sb = new System.Text.StringBuilder();//用于存储一定长度的接收数据（字符串），这种字符串数据结构特点是速度快
    public int receiveLength = (4+128*64+4 + 4+1+4 + 4+4+4 + 4+2+4 + 4+1+4) * 2;//指定sb的字符串长度，等于所有命令长度的2倍时最好 
    public int baseReceiveLength = (4 + 128 * 64 + 4 + 4 + 1 + 4 + 4 + 4 + 4 + 4 + 2 + 4 + 4 + 1 + 4) * 2;
    public bool flag = false;

    void Start()
    {
        OpenPort();//首先打开端口
        dataReceiveThread = new Thread(new ThreadStart(DataReceiveFunction));
        dataReceiveThread.Start();//启动读取数据的死循环线程
    }
    public void OpenPort()
    {
        //创建串口
        sp = new SerialPort(portName, baudRate, parity, dataBits, stopBits);
        sp.ReadTimeout = 400;
        try
        {
            sp.Open();
            Debug.Log("COM4已打开");
        }
        catch (Exception ex)
        {
            Debug.Log(ex.Message);
        }
    }

    //应用关闭时就关闭端口
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

    //接收数据的死循环线程
    void DataReceiveFunction()
    {       
        int bytes = 0;//用来判断读取是否成功，0是失败，1是成功
        while (true)
        {
            if (sp != null && sp.IsOpen)
            {
                try
                {
                    if (sp.BytesToRead == 0)
                    {
                        continue;
                    }//如果缓冲区中可以读取的字节数为0就跳过，否则按照可读取字节数创造Byte数组                
                    byte[] buffer = new byte[sp.BytesToRead];//按照可读取字节数创造Byte数组
                    Debug.Log(buffer.Length);//显示一下此时可读取字节的长度
                    bytes = sp.Read(buffer, 0, buffer.Length);//接收该长度的字节数据，存到Byte数组buffer里，bytes返回是否读取成功
                    if (bytes == 0)
                    {
                        continue;
                    }//读取失败，则跳过
                    else
                    {
                        string strbytes = Encoding.GetEncoding(37).GetString(buffer);
                        //将接受的数据buffer，从字节数据转为字符串，使用37号字符集（IBM EBCDIC (美国-加拿大)）或28591号字符集（ISO-8859-1），它们能保证1字节一个字符
                        //（当传过来的一个字节代表一个数据时，这种字符集的使用可以保留原始模样；
                        //如果传来的两个字节代表一个数据，可以使用utf-16；
                        //如果传来的就是ASCII编码的字符串，当然可以直接使用ASCII，即Encoding.ASCII.GetString(buffer)）
                        sb.Append(strbytes);//将转换后的字符串插入sb末尾
                        if (sb.Length > receiveLength)
                        {
                            sb.Remove(0, sb.Length - receiveLength);//用于保证sb始终长度为固定的值，并且只存最新的部分
                            flag = true;//用于指示sb长度已经达到固定的值
                        }                        
                    }//读取成功
                }
                catch (Exception ex)
                {
                    if (ex.GetType() != typeof(ThreadAbortException))
                    {
                        Debug.Log("读取出错了");
                    }
                }
            }
            //Thread.Sleep(1);//1ms后再循环，这样的等待时间可以保证115200波特率时，最快大约有100多字节存入4096字节的缓冲区
        }
    }

    //发送字符串(仅支持ASCII字符)
    public void WriteData(string dataStr)
    {
        if (sp.IsOpen)
        {
            sp.Write(dataStr);
        }
    }
    //发送字节数组
    public void WriteByte(byte[] buffer)
    {
        if (sp.IsOpen)
        {
            sp.Write(buffer, 0, buffer.Length);
        }
    }
}