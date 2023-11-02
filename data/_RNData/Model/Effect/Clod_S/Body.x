xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 11;
 -1.58420;0.83242;1.28154;,
 -1.35331;1.18314;-1.20927;,
 -1.57987;-1.30836;-1.25701;,
 -1.15298;-1.43755;1.16486;,
 1.13860;1.18749;-1.26774;,
 0.99479;-1.12044;-1.35252;,
 0.74236;0.96797;1.23226;,
 1.13860;-1.31251;0.94122;,
 0.99479;-1.12044;-1.35252;,
 1.13860;-1.31251;0.94122;,
 0.74236;0.96797;1.23226;;
 
 12;
 3;0,1,2;,
 3;0,2,3;,
 3;1,4,5;,
 3;1,5,2;,
 3;6,7,5;,
 3;6,5,4;,
 3;3,2,8;,
 3;3,8,9;,
 3;0,6,4;,
 3;0,4,1;,
 3;3,9,10;,
 3;3,10,0;;
 
 MeshMaterialList {
  1;
  12;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  18;
  0.997074;0.055595;0.052458;,
  0.983132;0.176801;-0.046834;,
  -0.997837;-0.051085;0.041378;,
  -0.968166;-0.192163;0.160401;,
  -0.992633;0.091779;-0.079091;,
  -0.023511;0.038160;-0.998995;,
  -0.031113;0.030415;-0.999053;,
  -0.038711;0.022666;-0.998993;,
  0.986345;-0.066987;0.150452;,
  0.058060;-0.995427;-0.075853;,
  0.070219;-0.995381;-0.065474;,
  0.045886;-0.995219;-0.086213;,
  -0.027413;0.993651;0.109118;,
  -0.056325;0.995324;0.078470;,
  0.001549;0.990211;0.139571;,
  0.063226;-0.077695;0.994970;,
  0.102413;-0.108398;0.988818;,
  0.023881;-0.046796;0.998619;;
  12;
  3;2,4,2;,
  3;2,2,3;,
  3;6,5,6;,
  3;6,6,7;,
  3;0,1,0;,
  3;0,0,8;,
  3;9,10,9;,
  3;9,9,11;,
  3;12,13,12;,
  3;12,12,14;,
  3;15,16,15;,
  3;15,15,17;;
 }
 MeshTextureCoords {
  11;
  0.176540;0.286630;,
  0.318700;0.587400;,
  0.108890;0.810420;,
  0.002160;0.474200;,
  0.519210;0.538150;,
  0.693630;0.765970;,
  0.602160;0.117810;,
  0.810960;0.440040;,
  -0.306370;0.765970;,
  -0.189040;0.440040;,
  -0.397840;0.117810;;
 }
}
