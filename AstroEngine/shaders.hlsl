cbuffer TimeBuffer : register(b0)
{
    float g_Time;
    float3 padding;
};

Texture2D myTexture : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT {
    float3 position    : POSITION;
    float4 color       : COLOR;
    float2 uv          : TEXCOORD;
    float3 instancePos : INSTANCE_POS;
    uint   instanceID  : SV_InstanceID;
};

struct VS_OUTPUT {
    float4 position : SV_POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD;
};

VS_OUTPUT VS_Main(VS_INPUT input) {
    VS_OUTPUT output;
    
    float scale = 1.0f + 0.5f * sin(g_Time + (float)input.instanceID);
    float3 scaledPos = input.position * scale;
    output.position = float4(scaledPos + input.instancePos, 1.0f);
    output.color = input.color;
    output.uv = input.uv;
    
    return output;
}

float4 PS_Main(VS_OUTPUT input) : SV_TARGET {
    float4 texColor = myTexture.Sample(textureSampler, input.uv);
    return texColor;
}