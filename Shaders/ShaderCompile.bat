dxc -spirv -T ps_6_0 -E main -Fo vert.spv VertexShader.hlsl -fspv-target-env=vulkan1.1
dxc -spirv -T ps_6_0 -E main -Fo frag.spv PixelShader.hlsl -fspv-target-env=vulkan1.1