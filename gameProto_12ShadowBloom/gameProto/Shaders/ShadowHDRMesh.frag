
// GLSL 3.3 を要求
#version 330

// fragment Shader 出力先を HDRBufferとHiBrightBufferの二つを指定
layout (location = 0) out vec4 HDRBuffer;
layout (location = 1) out vec4 HiBrightBuffer;

// 頂点シェーダーからの入力
// テクスチャ座標
in vec2 fragTexCoord;
// 法線（ワールド空間）
in vec3 fragNormal;
// 頂点位置（ワールド空間）
in vec3 fragWorldPos;
// ライト空間でのフラグメント位置(シャドウマップ)
in vec4 fragLightPos;

// テクスチャサンプリング
uniform sampler2D uDiffuseMap;
uniform sampler2D uNormalMap;
uniform sampler2D uSpecularMap;
uniform sampler2D uEmissiveMap;

// デプスマップ
uniform sampler2D depthMap;

// ディレクショナルライト用構造体
struct DirectionalLight
{
	// ライト方向
	vec3 mDirection;
	// ディフューズ色
	vec3 mDiffuseColor;
	// スペキュラー色
	vec3 mSpecColor;
};

// ライティング用変数
// カメラ位置（ワールド空間）
uniform vec3 uCameraPos;

// ポリゴン表面のスペキュラー強度
uniform float uSpecPower;

// アンビエントライト色
uniform vec3 uAmbientLight;

// 自己発光強度
uniform float uLuminance;

// ディレクショナルライト
uniform DirectionalLight uDirLight;

// シャドウ計算
float ShadowCalculation(vec4 fragPosLightSpace);

void main()
{
	// ポリゴン表面の法線（フラグメントシェーダー上で補間されている）
	vec3 N = normalize(fragNormal);
	// ポリゴン表面からライト方向へのベクトル
	vec3 L = normalize(-uDirLight.mDirection);
	// ポリゴン表面からカメラ方向
	vec3 V = normalize(uCameraPos - fragWorldPos);
	// -L ベクトルを 法線 N に対して反射したベクトルRを求める
	vec3 R = normalize(reflect(-L, N));

	// フォン反射計算
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);

	vec3 Diffuse;
	vec3 Specular;

	Diffuse = uDirLight.mDiffuseColor * max(NdotL,0.0f);
	Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);

	float shadow = ShadowCalculation(fragLightPos);

	// テクスチャカラーをディフューズ・アンビエントカラーに合成
	vec3 texColor = texture(uDiffuseMap, fragTexCoord).rgb;
	vec3 diffuseColor = vec3(Diffuse) * texColor;
	vec3 ambientColor = uAmbientLight * texColor;

	// シャドウを合成して描画（アンビエントはシャドウの3響を受けない）
	vec3 result = ( 1.0 - shadow ) * (diffuseColor + Specular) + ambientColor;
	
	//輝度計算によって高輝度成分のみ、HiBrightBufferに書き込み
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
    {
        HiBrightBuffer = vec4(result, 0.0f) + texture(uEmissiveMap, fragTexCoord) * uLuminance;
    }
    else
    {
        HiBrightBuffer = vec4(0.0f, 0.0f, 0.0f, 0.0f) + texture(uEmissiveMap, fragTexCoord) * uLuminance;
    }
	// HDR成分はそのままHDRバッファへ出力
    HDRBuffer = vec4(result, 0.0);
}

// シャドウ

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 0～1の範囲に変換
    projCoords = projCoords * 0.5 + 0.5;
    // デプスマップより深度取得 
    float closestDepth = texture(depthMap,projCoords.xy).r;
    // 現在のフラグメントのライト空間での深度値取得
    float currentDepth = projCoords.z;
    // 深度バイアス(デプスマップ解像度と勾配
    vec3 normal = normalize(fragNormal);
       vec3 lightDir = normalize(-uDirLight.mDirection);
    float bias = max(0.001 * (1.0 - dot(normal, lightDir)), 0.0001);
    
    // シャドウ判定
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    // デプスマップ範囲外は影なしにする
    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
    }
    return shadow;
}
