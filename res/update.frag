uniform sampler2D texture;
uniform vec2 texSize;
uniform bool isForOutput;
uniform vec2 mousePos;
uniform int functionId;

#define product(a, b) vec2(a.x*b.x-a.y*b.y, a.x*b.y+a.y*b.x)
#define conjugate(a) vec2(a.x,-a.y)
#define divide(a, b) vec2(((a.x*b.x+a.y*b.y)/(b.x*b.x+b.y*b.y)),((a.y*b.x-a.x*b.y)/(b.x*b.x+b.y*b.y)))
#define dist(veca, vecb) (  sqrt( (veca.x-vecb.x)*(veca.x-vecb.x) + (veca.y-vecb.y)*(veca.y-vecb.y) )  )



vec2 power(vec2 x, int powFactor)
{
  vec2 result = x;
  for (int i = 0; i < powFactor-1; ++i)
  {
    result = product(result,x);
  }
  return result;
}
vec2 func(vec2 z)
{
  switch(functionId)
  {
    case 4:
    {
      return conjugate(z);
    }
    case 3:
    {
      // f(z) = (z−2)**2  (z+1−2i)(z+2+2i)/z**3
      return divide( product( product(power(z-vec2(2.f,0.f),2), (z+vec2(1.f,-2.f))), (z+vec2(2.f,2.f)) ), power(z, 3));
    }
    case 2:
    {
      return z*2.f + vec2(0.f, 1.f);
    }
    case 1:
    {
      return power(z, 3);
    }
    case 0:
    {
      return power(z, 10) - z - vec2(1.f,0.f);
    }
    default:
    {
      return vec2(0.f);
    }
  }
  
} 


vec3 hsv2rgb(vec3 hsvIn)
{
    float hh, p, q, t, ff;
    int i;
    vec3 rgbOut;

    hh = hsvIn.x;
    hh /= 60.0;
    i = int(hh);
    ff = hh - float(i);
    p = hsvIn.z * (1.0 - hsvIn.y);
    q = hsvIn.z * (1.0 - (hsvIn.y * ff));
    t = hsvIn.z * (1.0 - (hsvIn.y * (1.0 - ff)));

    if(i == 0)
    {
      rgbOut.x = hsvIn.z;
      rgbOut.y = t;
      rgbOut.z = p;
    }
    if(i == 1)
    {
      rgbOut.x = q;
      rgbOut.y = hsvIn.z;
      rgbOut.z = p;
    }
    if(i == 2)
    {
      rgbOut.x = p;
      rgbOut.y = hsvIn.z;
      rgbOut.z = t;
    }
    if(i == 3)
    {
      rgbOut.x = p;
      rgbOut.y = q;
      rgbOut.z = hsvIn.z;
    }
    if(i == 4)
    {
      rgbOut.x = t;
      rgbOut.y = p;
      rgbOut.z = hsvIn.z;
    }
    if(i < 0 || i >= 5)
    {
      rgbOut.x = hsvIn.z;
      rgbOut.y = p;
      rgbOut.z = q;
    }
    return rgbOut;     
}


float atan2(float y, float x)
{
    float angle = x == 0.0 ? sign(abs(y))*1.57f : atan(abs(y), x);
    if (y < 0.f)
    {
      return (6.28f-angle)/6.28f;
    }
    return angle/6.28f;
}

void main()
{

    vec2 pos = gl_TexCoord[0].xy * texSize - texSize/2.f;
    vec2 realMousePos;
    if (!isForOutput)
    {
      pos = func(pos);
    }
    else
    {
      
      realMousePos = mousePos * texSize - texSize/2.f;
      realMousePos = func(realMousePos);
    }

    vec4 pixel = vec4(1.f);
    float hue = atan2(pos.y, pos.x) * 360.f;
    pixel = vec4(hsv2rgb(vec3(hue, 1.f, 1.f)), 1.f);
    float distFactor = sqrt(pos.x*pos.x + pos.y*pos.y) / sqrt( (texSize.x/2.f)*(texSize.x/2.f) + (texSize.y/2.f)*(texSize.y/2.f) );
    distFactor = 1.f - 1.f/(1.f + exp(distFactor*3.f));
    pixel *= distFactor;

    if (!isForOutput && dist( gl_TexCoord[0].xy , mousePos ) < 0.02f)
    {
      pixel.xyz = vec3(1.f) - pixel.xyz;
    }
    if (isForOutput && dist( pos , realMousePos ) < texSize.x * 0.02f)
    {
      pixel.xyz = vec3(1.f) - pixel.xyz;
    }

    gl_FragColor = gl_Color * pixel;
}