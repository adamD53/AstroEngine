struct VS_INPUT {
    float3 position : TEXCOORD0;
    float4 color    : TEXCOORD1;
    float2 uv       : TEXCOORD2;
};

struct VS_OUTPUT {
    float4 position : SV_POSITION;
    float4 color    : TEXCOORD0;
    float2 uv       : TEXCOORD1;
};

cbuffer UniformBlock : register(b0, space1) {
    column_major float4x4 u_MVP;
};

Texture2D<float4> u_Texture : register(t0, space2);
SamplerState      u_Sampler : register(s0, space2);

VS_OUTPUT VS_Main(VS_INPUT input) {
    VS_OUTPUT output;
    output.position = mul(u_MVP, float4(input.position, 1.0f));
    output.color    = input.color;
    output.uv       = input.uv;
    return output;
}

float4 PS_Main(VS_OUTPUT input) : SV_TARGET {
    return u_Texture.Sample(u_Sampler, input.uv) * input.color;
}
