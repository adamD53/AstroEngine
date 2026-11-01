struct VS_INPUT {
    float3 position : TEXCOORD0;
    float4 color    : TEXCOORD1;
};

struct VS_OUTPUT {
    float4 position : SV_POSITION;
    float4 color    : TEXCOORD0;
};

VS_OUTPUT VS_Main(VS_INPUT input) {
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    return output;
}

float4 PS_Main(VS_OUTPUT input) : SV_TARGET {
    return input.color;
}
