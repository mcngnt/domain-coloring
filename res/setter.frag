uniform sampler2D texture;
// uniform sampler2D oldState;
uniform vec2 texSize;
uniform vec2 cellPos;
uniform bool addCell;

#define dist(veca, vecb) (sqrt((veca.x-vecb.x)*(veca.x-vecb.x) + (veca.y-vecb.y)*(veca.y-vecb.y)))

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    if (addCell)
    {
    	vec2 pos = vec2(int(texSize.x*gl_TexCoord[0].x), int(texSize.y*gl_TexCoord[0].y));

    	if (dist(pos, cellPos) < 30.f)
    	{
    		pixel = vec4(1.f);
    	}
    }

    gl_FragColor = gl_Color * pixel;
}