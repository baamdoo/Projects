using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Barracuda;

public class AdaIN_Stylizer : MonoBehaviour
{
    public RenderTexture InputImage;
    public RenderTexture OutputImage;

    public ComputeShader ProcessShader;

    public List<Texture2D> StyleImages;

    public NNModel Encoder;
    public NNModel Decoder;
    public WorkerFactory.Type workerType = WorkerFactory.Type.Auto;

    private Model m_RuntimeEncoder;
    private Model m_RuntimeDecoder;

    private IWorker _engine;
    private IWorker _dengine;

    private bool _state;
    private int _index;
    private int _MAXIDX;

    void Start()
    {
        _state = false;
        _index = 0;
        _MAXIDX = StyleImages.Count;

        m_RuntimeEncoder = ModelLoader.Load(Encoder);
        m_RuntimeDecoder = ModelLoader.Load(Decoder);

        _engine = WorkerFactory.CreateWorker(workerType, m_RuntimeEncoder);
        _dengine = WorkerFactory.CreateWorker(workerType, m_RuntimeDecoder);
    }
    private void OnDisable()
    {
        _engine.Dispose();
        _dengine.Dispose();
    }

    private void ProcessImage(RenderTexture tex, string functionName)
    {
        int numThreads = 8;
        int kernelHandle = ProcessShader.FindKernel(functionName);
        RenderTexture tmpTex = RenderTexture.GetTemporary(tex.width, tex.height, 24, RenderTextureFormat.ARGBHalf);
        tmpTex.enableRandomWrite = true;
        tmpTex.Create();

        ProcessShader.SetTexture(kernelHandle, "Input", tex);
        ProcessShader.SetTexture(kernelHandle, "Result", tmpTex);

        ProcessShader.Dispatch(kernelHandle, tex.width / numThreads, tex.height / numThreads, 1);

        Graphics.Blit(tmpTex, tex);

        RenderTexture.ReleaseTemporary(tmpTex);
    }

    void Update()
    {
        if (Input.GetMouseButtonDown(1))
        {
            _state = !_state;
        }
        else if (Input.GetMouseButtonDown(2))
        {
            _index = (_index + 1) % _MAXIDX;
        }

        if (_state)
        {
            RenderTexture cTex = RenderTexture.GetTemporary(InputImage.width, InputImage.height, 24, RenderTextureFormat.ARGBHalf);
            RenderTexture sTex = RenderTexture.GetTemporary(InputImage.width, InputImage.height, 24, RenderTextureFormat.ARGBHalf);
            Graphics.Blit(InputImage, cTex);
            Graphics.Blit(InputImage, sTex);
            ProcessImage(cTex, "PreProcess");
            ProcessImage(sTex, "PreProcess");

            Tensor content = new Tensor(cTex, channels: 3);
            Tensor style = new Tensor(sTex, channels: 3);

            _engine.Execute(content);
            Tensor contentFeature = _engine.PeekOutput();
            content.Dispose();

            _engine.Execute(style);
            Tensor styleFeature = _engine.PeekOutput();
            style.Dispose();

            // Tensor feature = AdaIN(contentFeature, styleFeature);

            _dengine.Execute(styleFeature);
            Tensor output = _dengine.PeekOutput();

            RenderTexture.active = null;
            output.ToRenderTexture(cTex);
            output.Dispose();

            ProcessImage(cTex, "PostProcess");
            Graphics.Blit(cTex, OutputImage);

            RenderTexture.ReleaseTemporary(cTex);
            RenderTexture.ReleaseTemporary(sTex);
        }

        else
        {
            Graphics.Blit(InputImage, OutputImage);
        }
    }

    Tuple<Tensor, Tensor> InstanceNormalization(Tensor feature, float epsilon = 0.00001f)
    {
        Tensor sum = new Tensor(feature.batch, feature.channels);
        Tensor sum2 = new Tensor(feature.batch, feature.channels);
        Tensor mean = new Tensor(feature.batch, feature.channels);
        Tensor std = new Tensor(feature.batch, feature.channels);

        for (int h = 0; h < feature.height; h++)
        {
            for (int w = 0; w < feature.width; w++)
            {
                for (int c = 0; c < feature.channels; c++)
                {
                    sum[0, 0, 0, c] += feature[0, h, w, c];
                    sum2[0, 0, 0, c] += feature[0, h, w, c] * feature[0, h, w, c];
                }
            }
        }
        for (int c = 0; c < feature.channels; c++)
        {
            mean[0, 0, 0, c] = sum[0, 0, 0, c] / (feature.height * feature.width);
            std[0, 0, 0, c] = Mathf.Sqrt((sum2[0, 0, 0, c] / (feature.height * feature.width)) - mean[0, 0, 0, c] * mean[0, 0, 0, c] + epsilon);
        }

        Tuple<Tensor, Tensor> _params = new Tuple<Tensor, Tensor>(mean, std);
        return _params;
    }

    Tensor AdaIN(Tensor content, Tensor style)
    {
        Tuple<Tensor, Tensor> c_params = InstanceNormalization(content);
        Tuple<Tensor, Tensor> s_params = InstanceNormalization(style);

        Tensor result = new Tensor(content.shape);
        for (int h = 0; h < result.height; h++)
        {
            for (int w = 0; w < result.width; w++)
            {
                for (int c = 0; c < result.channels; c++)
                {
                    float normalize = (content[0, h, w, c] - c_params.Item1[0, 0, 0, c]) / c_params.Item2[0, 0, 0, c];
                    result[0, h, w, c] = normalize * s_params.Item2[0, 0, 0, c] + s_params.Item1[0, 0, 0, c];
                }
            }
        }
        return result;
    }
}
