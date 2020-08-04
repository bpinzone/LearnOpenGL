Composition hierarchy:

Game object has a
    model has many
        meshes have
            vertex data
            Material has
                Shader
                Data to put into uniforms (only that which its responsible for)
                    Textures
                    Other

TODO: Some members are still not private.