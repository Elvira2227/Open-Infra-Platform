//----------------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------------

cbuffer WorldBuffer
{
    float4x4 viewProjection;
    float4x4 projection;
    float4x4 view;
    float3 cam;
};

cbuffer SettingsBuffer : register(b1)
{
    float4 positions[4] : packoffset(c0);
    float pointSize : packoffset(c4);
    int bUseUniformPointSize : packoffset(c5);
    int bUseUniformColor: packoffset(c6);
};

cbuffer ViewportBuffer : register(b2)
{
    int2 viewport : packoffset(c0);
};

struct ApplicationToVertex
{
	float3 position			: Position;
	float4 color			: Color;
};

struct VertexToGeometry
{
	float3 worldPosition		: position;
	float4 color				: Color;
};

struct GeometryToFragment
{
	float4 position				: SV_POSITION;
	float2 uv					: Texture;
	float4 color				: Color;
};

struct FragmentToPixel
{
    float4 color : SV_Target0;
};

//----------------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------------

VertexToGeometry VS_main(ApplicationToVertex app2vs)
{
    VertexToGeometry vtf = (VertexToGeometry) 0;
    vtf.worldPosition = app2vs.position;
    vtf.color = bUseUniformColor ? float4(1, 1, 1, 1) : app2vs.color;
    return vtf;
}

//----------------------------------------------------------------------------
// Geometry Shader
//----------------------------------------------------------------------------
[maxvertexcount(4)]
void GS_main(point VertexToGeometry input[1], inout TriangleStream<GeometryToFragment> outStream)
{
	if (bUseUniformPointSize)
	{
        float fSizeX = (pointSize / viewport.x) / 4; //0.002;
        float fSizeY = (pointSize / viewport.y) / 4;

        GeometryToFragment output;
        float4 outPos = mul(view, float4(input[0].worldPosition, 1));
		for (int i = 0; i < 4; i++)
		{
            output.position = mul(projection,outPos);

			// ideal wäre wenn die pixel positon genau auf ein pixel-zentrum fallen würde
			float2 l_vCurrScreenPosition = output.position.xy / output.position.w;
			float z = output.position.z / output.position.w;
			float2 l_vCurrPixelPosition = l_vCurrScreenPosition * viewport.xy * 0.5;
			int x = l_vCurrPixelPosition.x;
			int y = l_vCurrPixelPosition.y;
			l_vCurrPixelPosition.x = x + 0.5;// round(l_vCurrPixelPosition.x);
			l_vCurrPixelPosition.y = y + 0.5;//round(l_vCurrPixelPosition.y);

            
			// wie wärde die ideelae Bildschirmposition [0;1]?
			l_vCurrScreenPosition = l_vCurrPixelPosition * (1.0f / (viewport.xy / 2)) * output.position.w;

			// nochmal - jetzt idealle Positon verwenden, aber alte Tiefe beibehalten
			l_vCurrScreenPosition = output.position.xy / output.position.w;
			output.position.xy = l_vCurrScreenPosition;
			output.position.w = 1;
			output.position.z = z;

			// Pixel nach der Projektion vergrößern auf gewünschte größe

			
			fSizeX = 1.0f / viewport.x * 2.0f * pointSize;
			fSizeY = 1.0f / viewport.y * 2.0f * pointSize;

			float4 posSize = float4(
                positions[i].x * fSizeX * 0.1f,
				positions[i].y * fSizeY * 0.1f,
				0,
				0);         

            output.position += posSize;

            output.color = float4(0,1,0,1);//input[0].color; //float3(204.0/255,204.0/255,204.0/255);
			
			output.uv.x = positions[i].x + 0.5;
			output.uv.y = 1.0 - positions[i].y + 0.5;
			
			outStream.Append(output);
		}
	}
	else
	{
        GeometryToFragment output;
        output.color = input[0].color;
        float4 pos = mul(viewProjection, float4(input[0].worldPosition, 1));
		for (int i = 0; i < 4; i++)
		{
			output.position = pos + float4(
				positions[i].x*pointSize * 0.01,
				positions[i].y*pointSize * 0.01,
				0,
				0);

			output.uv.x = positions[i].x + 0.5;
			output.uv.y = positions[i].y + 0.5;
			
		    outStream.Append(output);
		}
	}
	outStream.RestartStrip();
}


//----------------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------------

Texture2D<float4> texDiffuseMap : register(t0);
sampler sampler_ : register(s0);

FragmentToPixel PS_main(GeometryToFragment gtf)
{
    FragmentToPixel final;

    float4 texColor = float4(texDiffuseMap.Sample(sampler_, gtf.uv));
    final.color = texColor.rgba;

    return final;
}