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
 20;
 -0.61591;2.63926;3.53929;,
 -0.61591;-3.24724;7.75880;,
 10.30961;-1.81564;-3.16672;,
 2.84158;4.25487;-3.16672;,
 -7.02171;2.22396;-3.16672;,
 -11.54144;-3.44153;-3.16672;,
 -0.61591;-3.24724;7.75880;,
 -0.61591;2.63926;3.53929;,
 -7.02171;2.22396;-3.16672;,
 -0.61591;3.15541;-9.49434;,
 -0.61591;-1.81564;-14.09225;,
 -11.54144;-3.44153;-3.16672;,
 -0.61591;3.15541;-9.49434;,
 2.84158;4.25487;-3.16672;,
 10.30961;-1.81564;-3.16672;,
 -0.61591;-1.81564;-14.09225;,
 -0.61591;2.63926;3.53929;,
 2.84158;4.25487;-3.16672;,
 -0.61591;-3.24724;7.75880;,
 10.30961;-1.81564;-3.16672;;
 
 12;
 3;0,1,2;,
 3;0,2,3;,
 3;4,5,6;,
 3;4,6,7;,
 3;8,9,10;,
 3;8,10,11;,
 3;12,13,14;,
 3;12,14,15;,
 3;4,16,17;,
 3;4,17,12;,
 3;18,5,15;,
 3;18,15,19;;
 
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "Data\\Texture\\Material\\Leaf_Inside_1.PNG";
   }
  }
 }
 MeshNormals {
  16;
  0.213196;0.976619;0.027630;,
  0.583767;0.586489;0.561468;,
  0.360962;0.805843;0.469387;,
  0.592980;0.469117;0.654449;,
  -0.471191;0.804997;0.360498;,
  -0.619191;0.493969;0.610407;,
  -0.457500;0.820666;-0.342347;,
  -0.639824;0.518820;-0.566966;,
  -0.642161;0.465050;0.609392;,
  -0.664254;0.435495;0.607545;,
  -0.643745;0.513557;-0.567319;,
  0.583868;0.627395;-0.515242;,
  0.591777;0.547357;-0.591777;,
  0.074039;-0.995122;-0.065197;,
  0.082836;-0.994431;-0.065152;,
  0.065237;-0.995735;-0.065237;;
  12;
  3;2,3,1;,
  3;2,1,0;,
  3;4,5,8;,
  3;4,8,9;,
  3;7,6,7;,
  3;7,7,10;,
  3;11,0,11;,
  3;11,11,12;,
  3;4,2,0;,
  3;4,0,6;,
  3;13,14,13;,
  3;13,13,15;;
 }
 MeshTextureCoords {
  20;
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;;
 }
}
