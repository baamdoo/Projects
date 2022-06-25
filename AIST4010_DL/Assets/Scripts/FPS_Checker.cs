using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FPS_Checker : MonoBehaviour
{
    [Range(1, 100)]
    public int fontSize;
    [Range(0, 1)]
    public float Red, Green, Blue;

    private float _deltaTime = 0.0f;
    void Start()
    {
        fontSize = fontSize == 0 ? 50 : fontSize;
    }
    void Update()
    {
        _deltaTime += (Time.unscaledDeltaTime - _deltaTime) * 0.1f;
    }

    private void ColorRed()
    {
        Red = 1.0f; Green = 0.103f; Blue = 0.312f;
    }
    private void ColorGreen()
    {
        Red = 0.0f; Green = 1.0f; Blue = 0.112f;
    }
    private void ColorBlue()
    {
        Red = 0.182f; Green = 0.431f; Blue = 0.987f;
    }

    private void OnGUI()
    {
        GUIStyle style = new GUIStyle();
        Rect rect = new Rect(0, 0, Screen.width, Screen.height * 2 / 100);
        style.alignment = TextAnchor.UpperLeft;
        style.fontSize = Screen.height * 2 / fontSize;

        float msec = _deltaTime * 1000.0f;
        float fps = 1.0f / _deltaTime;
        string text = string.Format("{0:0.0}ms ({1:0.}fps)", msec, fps);

        if (fps > 60.0f)
            ColorGreen();
        else if (24.0f < fps && fps < 60.0f)
            ColorBlue();
        else
            ColorRed();

        style.normal.textColor = new Color(Red, Green, Blue, 1.0f);
        GUI.Label(rect, text, style);
    }
}
