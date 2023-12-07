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
 75;
 5.58760;0.00000;0.00000;,
 6.28604;-0.00000;0.92811;,
 0.00000;7.56000;0.92811;,
 0.00000;6.72000;0.00000;,
 6.98449;0.00000;0.00000;,
 0.00000;8.40000;0.00000;,
 6.28604;0.00000;-0.92811;,
 0.00000;7.56000;-0.92811;,
 5.58760;0.00000;0.00000;,
 0.00000;6.72000;0.00000;,
 -6.28604;0.00000;0.92811;,
 -5.58760;0.00000;0.00000;,
 -6.98449;0.00000;0.00000;,
 -6.28604;0.00000;-0.92811;,
 -5.58760;0.00000;0.00000;,
 0.00000;-7.56000;0.92811;,
 0.00000;-6.72000;-0.00000;,
 0.00000;-8.40000;-0.00000;,
 0.00000;-7.56000;-0.92811;,
 0.00000;-6.72000;-0.00000;,
 6.28604;-0.00000;0.92811;,
 5.58760;0.00000;0.00000;,
 6.98449;0.00000;0.00000;,
 6.28604;0.00000;-0.92811;,
 5.58760;0.00000;0.00000;,
 2.63790;0.00000;-2.63790;,
 3.65748;0.00000;-2.27780;,
 0.68984;5.04743;0.68984;,
 0.00000;4.48661;0.00000;,
 3.29738;0.00000;-3.29738;,
 0.00000;5.60826;0.00000;,
 2.27780;0.00000;-3.65748;,
 -0.68984;5.04743;-0.68984;,
 2.63790;0.00000;-2.63790;,
 0.00000;4.48661;0.00000;,
 -2.27780;0.00000;3.65748;,
 -2.63790;0.00000;2.63790;,
 -3.29738;0.00000;3.29738;,
 -3.65748;0.00000;2.27780;,
 -2.63790;0.00000;2.63790;,
 0.68984;-5.04743;0.68984;,
 0.00000;-4.48661;0.00000;,
 0.00000;-5.60826;0.00000;,
 -0.68984;-5.04743;-0.68984;,
 0.00000;-4.48661;0.00000;,
 3.65748;0.00000;-2.27780;,
 2.63790;0.00000;-2.63790;,
 3.29738;0.00000;-3.29738;,
 2.27780;0.00000;-3.65748;,
 2.63790;0.00000;-2.63790;,
 -1.50139;0.00000;-1.50139;,
 -1.10972;0.00000;-2.26841;,
 0.57934;2.87280;-0.57934;,
 0.00000;2.55360;0.00000;,
 -1.87674;0.00000;-1.87674;,
 0.00000;3.19200;0.00000;,
 -2.26841;0.00000;-1.10972;,
 -0.57934;2.87280;0.57934;,
 -1.50139;0.00000;-1.50139;,
 0.00000;2.55360;0.00000;,
 2.26841;-0.00000;1.10972;,
 1.50139;-0.00000;1.50139;,
 1.87674;-0.00000;1.87674;,
 1.10972;-0.00000;2.26841;,
 1.50139;-0.00000;1.50139;,
 0.57934;-2.87280;-0.57934;,
 -0.00000;-2.55360;0.00000;,
 -0.00000;-3.19200;0.00000;,
 -0.57934;-2.87280;0.57934;,
 -0.00000;-2.55360;0.00000;,
 -1.10972;0.00000;-2.26841;,
 -1.50139;0.00000;-1.50139;,
 -1.87674;0.00000;-1.87674;,
 -2.26841;0.00000;-1.10972;,
 -1.50139;0.00000;-1.50139;;
 
 48;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;3,2,10,11;,
 4;2,5,12,10;,
 4;5,7,13,12;,
 4;7,9,14,13;,
 4;11,10,15,16;,
 4;10,12,17,15;,
 4;12,13,18,17;,
 4;13,14,19,18;,
 4;16,15,20,21;,
 4;15,17,22,20;,
 4;17,18,23,22;,
 4;18,19,24,23;,
 4;25,26,27,28;,
 4;26,29,30,27;,
 4;29,31,32,30;,
 4;31,33,34,32;,
 4;28,27,35,36;,
 4;27,30,37,35;,
 4;30,32,38,37;,
 4;32,34,39,38;,
 4;36,35,40,41;,
 4;35,37,42,40;,
 4;37,38,43,42;,
 4;38,39,44,43;,
 4;41,40,45,46;,
 4;40,42,47,45;,
 4;42,43,48,47;,
 4;43,44,49,48;,
 4;50,51,52,53;,
 4;51,54,55,52;,
 4;54,56,57,55;,
 4;56,58,59,57;,
 4;53,52,60,61;,
 4;52,55,62,60;,
 4;55,57,63,62;,
 4;57,59,64,63;,
 4;61,60,65,66;,
 4;60,62,67,65;,
 4;62,63,68,67;,
 4;63,64,69,68;,
 4;66,65,70,71;,
 4;65,67,72,70;,
 4;67,68,73,72;,
 4;68,69,74,73;;
 
 MeshMaterialList {
  1;
  48;
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
  0,
  0,
  0,
  0;;
  Material {
   0.700000;0.672700;0.274400;1.000000;;
   3.000000;
   0.750000;0.750000;0.750000;;
   0.100000;0.096100;0.039200;;
   TextureFilename {
    "data\\TEXTURE\\gold.jpg";
   }
  }
 }
 MeshNormals {
  64;
  -0.665529;-0.553379;0.500841;,
  0.665528;0.553378;0.500844;,
  0.665528;0.553378;-0.500844;,
  0.665529;-0.553379;0.500841;,
  -0.665528;0.553378;0.500844;,
  -0.665528;0.553378;-0.500844;,
  0.665529;0.553379;0.500841;,
  -0.665528;-0.553378;0.500844;,
  -0.665528;-0.553378;-0.500844;,
  -0.543708;-0.639346;0.543708;,
  -0.266397;-0.600098;0.754264;,
  0.543708;0.639347;-0.543708;,
  0.266395;0.600098;-0.754266;,
  0.543708;-0.639346;-0.543708;,
  0.754264;-0.600098;-0.266397;,
  -0.543708;0.639347;0.543708;,
  -0.754266;0.600098;0.266395;,
  0.543708;0.639346;-0.543708;,
  0.754264;0.600098;-0.266397;,
  -0.543708;-0.639347;0.543708;,
  -0.754266;-0.600098;0.266395;,
  0.543708;-0.639346;0.543708;,
  0.698496;-0.620390;0.356679;,
  -0.543707;0.639347;-0.543707;,
  -0.698496;0.620391;-0.356678;,
  -0.543708;-0.639346;-0.543708;,
  -0.356679;-0.620390;-0.698496;,
  0.543707;0.639347;0.543707;,
  0.356678;0.620391;0.698496;,
  -0.543708;0.639346;-0.543708;,
  -0.356679;0.620390;-0.698496;,
  0.543707;-0.639347;0.543707;,
  0.356678;-0.620391;0.698496;,
  -0.665529;-0.553379;-0.500841;,
  0.665529;-0.553379;-0.500841;,
  0.665529;0.553379;-0.500841;,
  -0.665529;0.553379;0.500841;,
  0.665528;-0.553378;0.500844;,
  0.665528;-0.553378;-0.500844;,
  -0.665529;0.553379;-0.500841;,
  0.754266;0.600098;-0.266395;,
  -0.754264;-0.600098;0.266397;,
  -0.266395;0.600098;0.754266;,
  0.266397;-0.600098;-0.754264;,
  -0.266395;-0.600098;0.754266;,
  0.266397;0.600098;-0.754264;,
  -0.543708;0.639346;0.543708;,
  -0.266397;0.600098;0.754264;,
  0.754266;-0.600098;-0.266395;,
  0.543708;-0.639347;-0.543708;,
  0.266395;-0.600098;-0.754266;,
  -0.754264;0.600098;0.266397;,
  -0.356678;0.620391;-0.698496;,
  0.356679;-0.620390;0.698496;,
  0.698496;0.620391;0.356678;,
  -0.698496;-0.620390;-0.356679;,
  0.698496;-0.620391;0.356678;,
  -0.698496;0.620390;-0.356679;,
  0.543708;0.639346;0.543708;,
  0.698496;0.620390;0.356679;,
  -0.356678;-0.620391;-0.698496;,
  -0.543707;-0.639347;-0.543707;,
  -0.698496;-0.620391;-0.356678;,
  0.356679;0.620390;0.698496;;
  48;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;33,33,33,33;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;34,34,34,34;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;35,35,35,35;,
  4;36,36,36,36;,
  4;37,37,37,37;,
  4;38,38,38,38;,
  4;39,39,39,39;,
  4;9,10,10,9;,
  4;40,11,11,40;,
  4;11,12,12,11;,
  4;41,9,9,41;,
  4;13,14,14,13;,
  4;42,15,15,42;,
  4;15,16,16,15;,
  4;43,13,13,43;,
  4;17,18,18,17;,
  4;44,19,19,44;,
  4;19,20,20,19;,
  4;45,17,17,45;,
  4;46,47,47,46;,
  4;48,49,49,48;,
  4;49,50,50,49;,
  4;51,46,46,51;,
  4;21,22,22,21;,
  4;52,23,23,52;,
  4;23,24,24,23;,
  4;53,21,21,53;,
  4;25,26,26,25;,
  4;54,27,27,54;,
  4;27,28,28,27;,
  4;55,25,25,55;,
  4;29,30,30,29;,
  4;56,31,31,56;,
  4;31,32,32,31;,
  4;57,29,29,57;,
  4;58,59,59,58;,
  4;60,61,61,60;,
  4;61,62,62,61;,
  4;63,58,58,63;;
 }
 MeshTextureCoords {
  75;
  0.000000;0.000000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.250000;0.000000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.500000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.500000;0.750000;,
  0.500000;1.000000;,
  0.750000;0.250000;,
  0.750000;0.000000;,
  0.750000;0.500000;,
  0.750000;0.750000;,
  0.750000;1.000000;,
  1.000000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;0.750000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.250000;0.000000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.500000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.500000;0.750000;,
  0.500000;1.000000;,
  0.750000;0.250000;,
  0.750000;0.000000;,
  0.750000;0.500000;,
  0.750000;0.750000;,
  0.750000;1.000000;,
  1.000000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;0.750000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.250000;0.000000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.500000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.500000;0.750000;,
  0.500000;1.000000;,
  0.750000;0.250000;,
  0.750000;0.000000;,
  0.750000;0.500000;,
  0.750000;0.750000;,
  0.750000;1.000000;,
  1.000000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;0.750000;,
  1.000000;1.000000;;
 }
}