#pragma once

const char *simple_vert = "#version 330\r\n\r\nlayout (location = 1) in vec3 aPosition;\r\n\r\nvoid main() {\r\n    gl_Position = vec4(aPosition, 1.0);\r\n}";

const char *simple_frag = "#version 330\r\n\r\nout vec4 FragColor;\r\n\r\nuniform vec4 myColor;\r\n\r\nvoid main() {\r\n    FragColor = myColor;\r\n}";

const char *particle_vert = "#version 330\r\n\r\nlayout (location = 1) in vec3 aPosition;\r\nlayout (location = 2) in vec4 aColor;\r\nlayout (location = 3) in vec2 aTexCoord;\r\n\r\nout vec4 Color;\r\nout vec2 TexCoord;\r\n\r\nvoid main() {\r\n    gl_Position = vec4(aPosition, 1.0);\r\n    TexCoord = aTexCoord;\r\n    Color = aColor;\r\n}\r\n";

const char *particle_frag = "#version 330\r\n\r\nin vec4 Color;\r\nin vec2 TexCoord;\r\n\r\nout vec4 FragColor;\r\n\r\nuniform sampler2D gSampler;\r\n\r\nvoid main() {\r\n    FragColor = texture2D(gSampler, TexCoord.st) * Color;\r\n}\r\n";

const char *illumination_vert = "#version 330\r\n\r\nlayout (location = 1) in vec3 aPosition;\r\nlayout (location = 2) in vec3 aNormal;\r\nlayout (location = 3) in vec2 aTexCoord;\r\n\r\nout vec3 Position;\r\nout vec3 Normal;\r\nout vec2 TexCoord;\r\n\r\nvoid main()\r\n{\r\n    gl_Position = vec4(aPosition, 1.0);\r\n    Position = aPosition;\r\n    Normal = aNormal;\r\n    TexCoord = aTexCoord;\r\n}";

const char *illumination_frag = "#version 330\r\n\r\nin vec3 Normal;\r\nin vec3 Position;\r\nin vec2 TexCoord;\r\n\r\nout vec4 FragColor;\r\n\r\nstruct PointLight {\r\n    vec3 Color;\r\n    vec3 Position;\r\n};\r\n\r\nuniform int gUseTexture;\r\nuniform vec4 gTexColor;\r\nuniform sampler2D gSampler;\r\nuniform vec3 gEyePosition;\r\nuniform PointLight gPointLight;\r\n\r\nfloat gAmbientLight = 0.22f;\r\nfloat gDiffuseIntensity = 0.64f;\r\nfloat gSpecularIntensity = 0.82f;\r\nfloat gSpecularPower = 8.0f;\r\nfloat gConstant = 0.36f;\r\nfloat gLinear = 0.1f;\r\nfloat gExp = 0.1f;\r\n\r\nvec4 calcDiffuseLight(vec3 color, vec3 nor, vec3 direction, float intensity) {\r\n    float factor = dot(normalize(nor), normalize(-direction));\r\n    if (factor > 0) {\r\n        return vec4(color * intensity * factor, 1.0f);\r\n    } else {\r\n        return vec4(vec3(0.0f), 0.0f);\r\n    }\r\n}\r\n\r\nvec4 calcSpecularLight(vec3 color, vec3 nor, vec3 direction, vec3 eyePosition, float intensity, float power) {\r\n    float sign = dot(nor, -direction);\r\n    if (sign > 0) {\r\n        vec3 vertexToEye = normalize(eyePosition - Position);\r\n        vec3 reflection = normalize(reflect(direction, nor));\r\n        float factor = dot(vertexToEye, reflection);\r\n        if (factor > 0) {\r\n            factor = pow(factor, power);\r\n            return vec4(color * intensity * factor, 1.0f);\r\n        } else {\r\n            return vec4(vec3(0.0f), 0.0f);\r\n        }\r\n    }\r\n    return vec4(vec3(0.0f), 0.0f);\r\n}\r\n\r\nvec4 calcPointLight(PointLight light) {\r\n    vec3 lightDirection = Position - light.Position;\r\n    float distance = length(lightDirection);\r\n    vec4 diffuseLignt = calcDiffuseLight(light.Color, Normal, lightDirection, gDiffuseIntensity);\r\n    vec4 specularLight = calcSpecularLight(light.Color, Normal, lightDirection, gEyePosition, gSpecularIntensity, gSpecularPower);\r\n    float attenutation = gConstant + gLinear * distance + gExp * distance * distance;\r\n    return (diffuseLignt + specularLight) / attenutation;\r\n}\r\n\r\nvoid main()\r\n{\r\n    vec4 color = gTexColor + vec4(gAmbientLight);\r\n    if (gUseTexture > 0) {\r\n        FragColor = color * texture2D(gSampler, TexCoord.st) * calcPointLight(gPointLight);\r\n    } else {\r\n        FragColor = color * calcPointLight(gPointLight);\r\n    }\r\n}";
