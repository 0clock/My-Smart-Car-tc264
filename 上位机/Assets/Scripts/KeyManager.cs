using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class KeyManager : MonoBehaviour
{
    public WhiteBoard whiteBoard;
    float a_Speed = 0.05F;
    float b_Steering = 5F;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (whiteBoard.key_Control == true)
        {
            if (Input.GetKey(KeyCode.W))
            {
                whiteBoard.speed_Target += a_Speed;
                GameObject.Find("UI/Canvas/Text (10)/Slider").GetComponent<Slider>().value = whiteBoard.speed_Target;
            }
            if (Input.GetKey(KeyCode.S))
            {
                whiteBoard.speed_Target -= a_Speed;
                GameObject.Find("UI/Canvas/Text (10)/Slider").GetComponent<Slider>().value = whiteBoard.speed_Target;
            }
            if (Input.GetKey(KeyCode.A))
            {              
                whiteBoard.steering_Target -= b_Steering;
                GameObject.Find("UI/Canvas/Text (11)/Slider").GetComponent<Slider>().value = whiteBoard.steering_Target;
            }
            if (Input.GetKey(KeyCode.D))
            {
                whiteBoard.steering_Target += b_Steering;
                GameObject.Find("UI/Canvas/Text (11)/Slider").GetComponent<Slider>().value = whiteBoard.steering_Target;
            }
            if (Input.GetKey(KeyCode.P))
            {
                whiteBoard.speed_Target = whiteBoard.speed_Target/2;
                GameObject.Find("UI/Canvas/Text (10)/Slider").GetComponent<Slider>().value = whiteBoard.speed_Target;
            }

            whiteBoard.speed_Target = whiteBoard.speed_Target - a_Speed * whiteBoard.speed_Target / (3 + a_Speed);
            GameObject.Find("UI/Canvas/Text (10)/Slider").GetComponent<Slider>().value = whiteBoard.speed_Target;
            whiteBoard.steering_Target = whiteBoard.steering_Target - b_Steering * whiteBoard.steering_Target / (30 + b_Steering);
            GameObject.Find("UI/Canvas/Text (11)/Slider").GetComponent<Slider>().value = whiteBoard.steering_Target;
            if (whiteBoard.speed_Target < 0.5*a_Speed && whiteBoard.speed_Target > -0.5 * a_Speed)
            {
                whiteBoard.speed_Target = 0;
                GameObject.Find("UI/Canvas/Text (10)/Slider").GetComponent<Slider>().value = whiteBoard.speed_Target;
            }
            if (whiteBoard.steering_Target < 0.5 * b_Steering && whiteBoard.steering_Target > -0.5 * b_Steering)
            {
                whiteBoard.steering_Target = 0;
                GameObject.Find("UI/Canvas/Text (11)/Slider").GetComponent<Slider>().value = whiteBoard.steering_Target;
            }
        }

    }
}
