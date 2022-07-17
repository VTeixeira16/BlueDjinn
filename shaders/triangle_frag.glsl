#version 330 core
//Processa a aparencia

uniform sampler2D TextureSampler;
uniform sampler2D CloudsTexture;

uniform float Time;

uniform vec2 CloudsRotationSpeed = vec2(0.01, 0.0);

in vec3 Normal;
in vec3 Color;
in vec2 UV;

uniform vec3 LightDirection;
uniform float LightIntensity;

out vec4 OutColor;

void main()
{
    //Renormalizar a normal para evitar problemas com a interpolacao linear
    vec3 N = normalize(Normal);

    //Inverter a direção da luz para calcular o vetor L
    vec3 L = -normalize(LightDirection);

    float Lambertian = max(dot(N, L), 0.0);

    // Vetor V
    vec3 ViewDirection = vec3(0.0, 0.0, -1.0);
    vec3 V = -ViewDirection;

    //Vetor R
    vec3 R = reflect(-L, N);

    //Termo Especular (R . V) ^ alpha
    float Alpha = 50.0;
    float Specular = pow(max(dot(R, V), 0.0), Alpha);
    Specular = max(Specular, 0.0);


//    OutColor = vec4(Color, 1.0);
//    float ColorIntensity = 1.0f;
    vec3 EarthColor = texture(TextureSampler, UV).rgb;
    vec3 CloudColor = texture(CloudsTexture, UV + Time * CloudsRotationSpeed).rgb;
//    vec3 FinalColor = ColorIntensity * Color * TextureColor;
    vec3 FinalColor = (EarthColor + CloudColor) * LightIntensity * Lambertian + Specular;


//    OutColor = vec4(UV, 0.0, 1.0);
    OutColor = vec4(FinalColor, 1.0);
//    OutColor = vec4(normalize(Normal), 1.0);


}
