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
 17.52047;4.09379;-3.16672;,
 1.76846;5.55548;9.74857;,
 1.76846;-3.93157;18.68433;,
 23.61951;-1.09626;-3.16672;,
 1.76846;5.55548;9.74857;,
 -7.61298;8.53658;-3.16672;,
 -20.08260;-1.09626;-3.16672;,
 1.76846;-3.93157;18.68433;,
 1.76846;5.55548;-14.97508;,
 1.76846;-2.62542;-25.01778;,
 -20.08260;-1.09626;-3.16672;,
 -7.61298;8.53658;-3.16672;,
 17.52047;4.09379;-3.16672;,
 23.61951;-1.09626;-3.16672;,
 1.76846;-2.62542;-25.01778;,
 1.76846;5.55548;-14.97508;,
 1.76846;5.55548;9.74857;,
 17.52047;4.09379;-3.16672;,
 23.61951;-1.09626;-3.16672;,
 1.76846;-3.93157;18.68433;;
 
 12;
 3;0,1,2;,
 3;0,2,3;,
 3;4,5,6;,
 3;4,6,7;,
 3;8,9,10;,
 3;8,10,11;,
 3;12,13,14;,
 3;12,14,15;,
 3;5,16,17;,
 3;5,17,15;,
 3;18,19,6;,
 3;18,6,14;;
 
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
  0.388379;0.861129;0.328052;,
  0.519233;0.610170;0.598406;,
  0.390868;0.837464;0.381938;,
  0.535345;0.591332;0.603102;,
  -0.063964;0.978863;0.194257;,
  -0.529272;0.625399;0.573364;,
  -0.100962;0.854838;-0.508977;,
  -0.012135;0.775255;-0.631532;,
  -0.538440;0.577764;0.613409;,
  -0.502157;0.662152;-0.556232;,
  -0.504243;0.652738;-0.565396;,
  0.505099;0.652599;-0.564791;,
  0.530581;0.623507;-0.574215;,
  -0.000000;-0.999562;-0.029580;,
  -0.000000;-0.991687;-0.128677;,
  0.000000;-0.997560;0.069810;;
  12;
  3;0,2,3;,
  3;0,3,1;,
  3;5,4,5;,
  3;5,5,8;,
  3;6,7,9;,
  3;6,9,10;,
  3;11,12,12;,
  3;11,7,6;,
  3;4,2,0;,
  3;4,0,6;,
  3;13,14,13;,
  3;13,13,15;;
 }
 MeshTextureCoords {
  20;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
