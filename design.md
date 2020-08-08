OOP Composition hierarchy:
* Game object has a
    * Model has many
        * Meshes have
            * Vertex data
            * Material has
                * Shader
                * Data to put into uniforms (only that which its responsible for)
                    * Textures
                    * Other

TODO: Some members are still not private.