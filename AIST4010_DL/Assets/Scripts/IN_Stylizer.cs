using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Barracuda;

public class IN_Stylizer : MonoBehaviour
{
    public RenderTexture InputImage;
    public RenderTexture OutputImage;

    public ComputeShader ProcessShader;

    public List<NNModel> ModelAssets;

    public WorkerFactory.Type workerType = WorkerFactory.Type.Auto;

    private Model m_RuntimeModel;
    private IWorker _engine;

    private bool _state;
    private int _index;
    private int _MAXIDX;

    void Start()
    {
        _state = false;
        _index = 0;
        _MAXIDX = ModelAssets.Count;

        m_RuntimeModel = ModelLoader.Load(ModelAssets[_index]);

        _engine = WorkerFactory.CreateWorker(workerType, m_RuntimeModel);
    }
    private void OnDisable()
    {
        _engine.Dispose();
    }

    private void ProcessImage(ref RenderTexture tex, string functionName)
    {
        int numThreads = 8;
        int kernelHandle = ProcessShader.FindKernel(functionName);
        RenderTexture tmpTex = RenderTexture.GetTemporary(tex.width, tex.height, 24, RenderTextureFormat.ARGBHalf);
        tmpTex.enableRandomWrite = true;
        tmpTex.Create();

        ProcessShader.SetTexture(kernelHandle, "Result", tmpTex);
        ProcessShader.SetTexture(kernelHandle, "Input", tex);

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

            m_RuntimeModel = ModelLoader.Load(ModelAssets[_index]);
            _engine = WorkerFactory.CreateWorker(workerType, m_RuntimeModel);
        }

        if (_state)
        {
            RenderTexture tmpTex = RenderTexture.GetTemporary(InputImage.width, InputImage.height, 24, RenderTextureFormat.ARGBHalf);
            Graphics.Blit(InputImage, tmpTex); // Copy InputImage -> tmpTex
            ProcessImage(ref tmpTex, "PreProcess");

            Tensor input = new Tensor(tmpTex, channels: 3);

            _engine.Execute(input); // Inference
            Tensor output = _engine.PeekOutput(); // Get result
            input.Dispose();

            RenderTexture.active = null;
            output.ToRenderTexture(tmpTex); // Copy output -> tmpTex with casting
            output.Dispose();

            ProcessImage(ref tmpTex, "PostProcess");
            Graphics.Blit(tmpTex, OutputImage); // Copy tmpTex -> OutputImage

            RenderTexture.ReleaseTemporary(tmpTex);
        }

        else
        {
            Graphics.Blit(InputImage, OutputImage);
        }
    }
}
