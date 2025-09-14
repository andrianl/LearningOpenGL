#shader vertex
#version 460 core

// Input attributes coming from your vertex buffer
layout (location = 0) in vec3 inputPosition;
layout (location = 1) in vec3 inputNormal;

// Data we pass to the fragment shader
out VertexToFragment {
    vec3 worldPosition;  // Position of the vertex in world space
    vec3 worldNormal;    // Normal of the vertex in world space (unit length)
} vertexToFragment;

// Uniform matrices provided by your application
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    // Transform the vertex position from model space into world space
    vec4 worldPosition4 = modelMatrix * vec4(inputPosition, 1.0);
    vertexToFragment.worldPosition = worldPosition4.xyz;

    // Build the normal matrix to correctly transform normals with non-uniform scale
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));

    // Transform the vertex normal into world space and normalize it
    vertexToFragment.worldNormal = normalize(normalMatrix * inputNormal);

    // Compute the final position on the screen (clip space)
    gl_Position = projectionMatrix * viewMatrix * worldPosition4;
}

#shader pixel
#version 460 core

// Data coming from the vertex shader
in VertexToFragment {
    vec3 worldPosition;  // The position of the fragment in world space
    vec3 worldNormal;    // The normal of the fragment in world space
} vertexToFragment;

// Final color output
out vec4 outputColor;

// A simple point light description
struct PointLight {
    vec3 position;   // Light position in world space
    vec3 color;      // Light intensity in linear color space (for example, 1,1,1 is white)
};

// A Disney �Principled� material with the most common controls
struct DisneyMaterial {
    vec3 baseColor;        // The base color of the material (0..1)
    float metallic;        // 0 for dielectric (plastic, wood), 1 for metal
    float roughness;       // 0 is smooth mirror, 1 is very rough
    float specular;        // Dielectric reflectivity control (affects F0 ~ 0.08 * specular^2)
    float specularTint;    // Tints the specular for dielectrics
    float sheen;           // Soft fabric-like highlight amount
    float sheenTint;       // Tints the sheen towards baseColor hue
    float clearCoat;       // Additional thin glossy layer on top (0..1)
    float clearCoatGloss;  // How glossy the clear coat is (0..1)
};

// Uniforms provided by your application
uniform PointLight pointLight;
uniform DisneyMaterial material;
uniform vec3 cameraWorldPosition;

// Small helpers
float clampToUnit(float value) { return clamp(value, 0.0, 1.0); }
vec3 clampToUnit(vec3 value) { return clamp(value, 0.0, 1.0); }

// Compute perceived luminance (used to extract a neutral tint)
float computeLuminance(vec3 colorLinear)
{
    // These weights match human eye sensitivity for red, green, blue
    return dot(colorLinear, vec3(0.2126, 0.7152, 0.0722));
}

// Fresnel using the Schlick approximation with optional tint for dielectrics
vec3 fresnelSchlickWithTint(float cosineOfAngle, vec3 reflectanceAtNormalIncidence, float specularTint, vec3 baseColor)
{
    // Build a unit color that preserves the hue of the base color
    float baseLuminance = max(1e-5, computeLuminance(baseColor));
    vec3 unitTintColor = baseColor / baseLuminance;

    // For dielectrics, allow tint to bias the �white� Fresnel towards the base color hue
    vec3 whiteColor = vec3(1.0);
    vec3 tintedWhite = mix(whiteColor, unitTintColor, specularTint);

    // Standard Schlick curve: mix between the reflectance and white as the angle becomes more grazing
    float oneMinusCos = 1.0 - cosineOfAngle;
    float schlickFactor = pow(oneMinusCos, 5.0);

    // We bias the curve with the tinted white to obtain Disney�s �specularTint� behavior
    // This keeps metal behavior (where reflectanceAtNormalIncidence comes from baseColor) and
    // gives a pleasant artistic control for dielectrics.
    return mix(reflectanceAtNormalIncidence * tintedWhite, whiteColor, schlickFactor);
}

// Microfacet normal distribution (GGX, also known as Trowbridge-Reitz)
// Controls how many microfacets are aligned with the half vector
float normalDistributionGGX(float cosineNormalHalf, float microfacetSlope)
{
    // microfacetSlope is often called �alpha� and is roughness squared
    float slopeSquared = microfacetSlope * microfacetSlope;
    float denominatorHelper = (cosineNormalHalf * cosineNormalHalf) * (slopeSquared - 1.0) + 1.0;
    float denominator = 3.14159265 * denominatorHelper * denominatorHelper;
    return slopeSquared / max(denominator, 1e-6);
}

// Geometry masking and shadowing using Smith�s method with a Schlick-like approximation
float smithMaskingShadowingSingle(float cosineNormalDirection, float microfacetSlope)
{
    float k = (microfacetSlope * microfacetSlope) * 0.5; // Schlick-Smith approximation factor
    return cosineNormalDirection / (cosineNormalDirection * (1.0 - k) + k);
}

float smithMaskingShadowingCombined(float cosineNormalLight, float cosineNormalView, float microfacetSlope)
{
    return smithMaskingShadowingSingle(cosineNormalLight, microfacetSlope)
         * smithMaskingShadowingSingle(cosineNormalView, microfacetSlope);
}

// Disney diffuse (Burley) improves on simple Lambert for rough surfaces and grazing angles
float disneyDiffuseFactor(float cosineNormalLight, float cosineNormalView, float cosineLightHalf, float perceptualRoughness)
{
    float facingLight = pow(1.0 - cosineNormalLight, 5.0);
    float facingView  = pow(1.0 - cosineNormalView, 5.0);

    // As roughness grows, retro-reflection increases (Fd90 term)
    float diffuseAtGrazing = 0.5 + 2.0 * cosineLightHalf * cosineLightHalf * perceptualRoughness;

    float lightScatterFactor = 1.0 + (diffuseAtGrazing - 1.0) * facingLight;
    float viewScatterFactor  = 1.0 + (diffuseAtGrazing - 1.0) * facingView;

    return lightScatterFactor * viewScatterFactor;
}

// Soft fabric-like sheen highlight
vec3 disneySheenColor(float cosineLightHalf, float sheenAmount, float sheenTint, vec3 baseColor)
{
    if (sheenAmount <= 0.0) return vec3(0.0);

    // Build a unit tint from base color
    float baseLuminance = max(1e-5, computeLuminance(baseColor));
    vec3 unitTintColor = baseColor / baseLuminance;

    vec3 finalSheenTint = mix(vec3(1.0), unitTintColor, sheenTint);

    // Use Schlick-like falloff for the half-angle
    float oneMinusCos = 1.0 - cosineLightHalf;
    float sheenFalloff = pow(oneMinusCos, 5.0);

    return sheenAmount * sheenFalloff * finalSheenTint;
}

// Clear coat top layer: very glossy, thin transparent coat using GTR1 distribution
float normalDistributionGTR1(float cosineNormalHalf, float coatGlossSlope)
{
    // GTR1 uses a log term; we guard against tiny values to avoid numerical issues
    float slope = max(coatGlossSlope, 1e-4);
    float slopeSquared = slope * slope;

    float t = 1.0 + (slopeSquared - 1.0) * (cosineNormalHalf * cosineNormalHalf);
    float denominator = 3.14159265 * log(slopeSquared) * t;

    return (slopeSquared - 1.0) / max(denominator, 1e-6);
}

void main()
{
    // Read and normalize the surface normal in world space
    vec3 surfaceNormal = normalize(vertexToFragment.worldNormal);

    // Compute view direction (from fragment to camera) and normalize
    vec3 viewDirection = normalize(cameraWorldPosition - vertexToFragment.worldPosition);

    // Compute light direction (from fragment to light) and normalize
    vec3 lightDirection = normalize(pointLight.position - vertexToFragment.worldPosition);

    // Half vector is the normalized average of light and view directions
    vec3 halfVector = normalize(lightDirection + viewDirection);

    // Cosines for all the important pairs (clamped to [0,1] for stability)
    float cosineNormalLight = clampToUnit(dot(surfaceNormal, lightDirection));
    float cosineNormalView  = clampToUnit(dot(surfaceNormal, viewDirection));
    float cosineNormalHalf  = clampToUnit(dot(surfaceNormal, halfVector));
    float cosineLightHalf   = clampToUnit(dot(lightDirection, halfVector));

    // If the surface does not face the light or the camera, we return black
    if (cosineNormalLight <= 0.0 || cosineNormalView <= 0.0)
    {
        outputColor = vec4(0.0);
        return;
    }

    // Convert artist-friendly perceptual roughness into microfacet slope (often called �alpha�)
    float perceptualRoughness = clamp(material.roughness, 0.0, 1.0);
    float microfacetSlope = max(1e-4, perceptualRoughness * perceptualRoughness);

    // Base reflectance at normal incidence (F0)
    // For dielectrics, F0 is a small gray; for metals, F0 is the base color.
    float dielectricSpecularControl = material.specular; // 0..1
    float dielectricF0Scalar = 0.08 * dielectricSpecularControl * dielectricSpecularControl;
    vec3 reflectanceAtNormalIncidence = mix(vec3(dielectricF0Scalar), material.baseColor, material.metallic);

    // Fresnel term with optional tint for dielectrics
    vec3 fresnelTerm = fresnelSchlickWithTint(cosineLightHalf, reflectanceAtNormalIncidence,
                                              material.specularTint, material.baseColor);

    // Microfacet specular using GGX distribution and Smith masking-shadowing
    float normalDistribution = normalDistributionGGX(cosineNormalHalf, microfacetSlope);
    float geometryMaskingShadowing = smithMaskingShadowingCombined(cosineNormalLight, cosineNormalView, microfacetSlope);

    // Denominator ensures proper energy scaling for reflection geometry
    float microfacetDenominator = max(4.0 * cosineNormalLight * cosineNormalView, 1e-6);

    // Final specular reflectance from the microfacet model
    vec3 specularReflection = (normalDistribution * geometryMaskingShadowing) * fresnelTerm / microfacetDenominator;

    // Diffuse color only exists for dielectrics (metals have almost no diffuse)
    vec3 diffuseAlbedo = material.baseColor * (1.0 - material.metallic);

    // Disney diffuse factor (Burley�s model)
    float disneyDiffuse = disneyDiffuseFactor(cosineNormalLight, cosineNormalView, cosineLightHalf, perceptualRoughness);

    // Convert diffuse to reflectance by dividing by pi (Lambertian scaling)
    vec3 diffuseReflection = diffuseAlbedo * (disneyDiffuse / 3.14159265);

    // Sheen contribution (soft, fabric-like lobe)
    vec3 sheenReflection = disneySheenColor(cosineLightHalf, material.sheen, material.sheenTint, material.baseColor);

    // Clear coat: a second very glossy specular lobe sitting on top of everything
    float clearCoatStrength = material.clearCoat * 0.25; // usually subtle
    float clearCoatSlope = mix(0.1, 0.001, material.clearCoatGloss); // smaller is glossier
    float clearCoatDistribution = normalDistributionGTR1(cosineNormalHalf, clearCoatSlope);

    // For clear coat we reuse a fixed geometry factor (very glossy, thin layer)
    float clearCoatGeometry = smithMaskingShadowingCombined(cosineNormalLight, cosineNormalView, 0.25);

    // Clear coat Fresnel: simple Schlick with F0 around 0.04 (typical for clear varnish)
    float clearCoatFresnel = mix(0.04, 1.0, pow(1.0 - cosineLightHalf, 5.0));

    float clearCoatSpecular = clearCoatStrength
                            * (clearCoatDistribution * clearCoatGeometry * clearCoatFresnel)
                            / microfacetDenominator;

    // Energy conservation: reduce diffuse by average Fresnel so total reflected energy stays plausible
    float averageFresnelForMix = mix(dielectricF0Scalar, 1.0, material.metallic);
    diffuseReflection *= (1.0 - averageFresnelForMix);

    // Sum all reflectance lobes
    vec3 totalBidirectionalReflectance = diffuseReflection
                                       + specularReflection
                                       + sheenReflection
                                       + vec3(clearCoatSpecular);

    // Light incoming radiance (color) times cosine term gives the actual energy from the light
    vec3 incomingLightRadiance = pointLight.color;
    vec3 outgoingRadiance = totalBidirectionalReflectance * incomingLightRadiance * cosineNormalLight;

    // Output the final color (clamped to the displayable range)
    outputColor = vec4(clampToUnit(outgoingRadiance), 1.0);
}