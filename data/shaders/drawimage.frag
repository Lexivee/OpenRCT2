#version 150

const int FLAG_COLOUR              = (1 << 0);
const int FLAG_REMAP               = (1 << 1);
const int FLAG_TRANSPARENT         = (1 << 2);
const int FLAG_TRANSPARENT_SPECIAL = (1 << 3);

uniform usampler2DArray uTexture;

flat in ivec4   fClip;
flat in int     fFlags;
flat in uint    fColour;
flat in int     fTexColourAtlas;
flat in int     fTexMaskAtlas;
flat in int     fTexPaletteAtlas;
flat in vec4    fTexPaletteBounds;
flat in int     fMask;

in vec2         fPosition;
in vec2         fTexColourCoords;
in vec2         fTexMaskCoords;

out uint oColour;

void main()
{
    if (fPosition.x < fClip.x || fPosition.x > fClip.z ||
        fPosition.y < fClip.y || fPosition.y > fClip.w)
    {
        discard;
    }

    uint texel;

    // If remap palette used
    if ((fFlags & FLAG_REMAP) != 0)
    {
        // z is the size of each x pixel in the atlas
        float x = fTexPaletteBounds.x + texture(uTexture, vec3(fTexColourCoords, float(fTexColourAtlas))).r * fTexPaletteBounds.z;
        texel = texture(uTexture, vec3(x, fTexPaletteBounds.y, float(fTexPaletteAtlas))).r;
    } // If transparent or special transparent
    else if ((fFlags & (FLAG_TRANSPARENT | FLAG_TRANSPARENT_SPECIAL)) != 0)
    {
        uint line = texture(uTexture, vec3(fTexColourCoords, float(fTexColourAtlas))).r;
        if (line == 0u)
        {
            discard;
        }
        /*
        float alpha = 0.5;
        if ((fFlags & FLAG_TRANSPARENT_SPECIAL) != 0)
        {
            alpha = 0.5 + (line - 1.0) / 10.0;
        }
         */
    
        // z is the size of each x pixel in the atlas
        float x = fTexPaletteBounds.x + fTexPaletteBounds.z * 50.0;
        //oColour = vec4(uPalette[texture(uTexture, vec3(x, fTexPaletteBounds.y, float(fTexPaletteAtlas))).r].rgb, alpha);
        oColour = texture(uTexture, vec3(x, fTexPaletteBounds.y, float(fTexPaletteAtlas))).r;
        
        return;
    }
    else
    {
        texel = texture(uTexture, vec3(fTexColourCoords, float(fTexColourAtlas))).r;
    }

    if (fMask != 0)
    {
        uint mask = texture(uTexture, vec3(fTexMaskCoords, float(fTexMaskAtlas))).r;
        if ( mask == 0u )
        {
            discard;
        }

        oColour = texel;
    }
    else
    {
        if (texel == 0u)
        {
            discard;
        }
        if ((fFlags & FLAG_COLOUR) != 0)
        {
            //oColour = vec4(fColour.rgb, fColour.a * texel.a);
            oColour = fColour;
        }
        else
        {
            oColour = texel;
        }
    }
}
