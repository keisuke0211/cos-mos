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
 71;
 0.00000;-8.22537;25.15758;,
 0.00000;2.15817;17.25880;,
 -12.20380;2.15817;12.20380;,
 -17.78910;-8.22537;17.78910;,
 -17.25880;2.15817;0.00000;,
 -25.15758;-8.22537;0.00000;,
 -12.20380;2.15817;-12.20380;,
 -17.78910;-8.22537;-17.78910;,
 0.00000;2.15817;-17.25880;,
 -0.00000;-8.22537;-25.15758;,
 12.20382;2.15817;-12.20380;,
 17.78910;-8.22537;-17.78908;,
 17.25880;2.15817;0.00000;,
 25.15758;-8.22537;0.00000;,
 12.20382;2.15817;12.20380;,
 17.78910;-8.22537;17.78908;,
 0.00000;2.15817;17.25880;,
 0.00000;-8.22537;25.15758;,
 0.00000;7.71531;13.53432;,
 -9.57020;7.71531;9.57020;,
 -13.53432;7.71531;0.00000;,
 -9.57020;7.71531;-9.57020;,
 0.00000;7.71531;-13.53432;,
 9.57020;7.71531;-9.57020;,
 13.53432;7.71531;0.00000;,
 9.57020;7.71531;9.57020;,
 0.00000;7.71531;13.53432;,
 0.00000;11.51537;11.21550;,
 -7.93056;11.51537;7.93056;,
 -11.21550;11.51537;0.00000;,
 -7.93056;11.51537;-7.93056;,
 0.00000;11.51537;-11.21550;,
 7.93056;11.51537;-7.93056;,
 11.21550;11.51537;0.00000;,
 7.93056;11.51537;7.93056;,
 0.00000;11.51537;11.21550;,
 0.00000;21.18952;6.21702;,
 -5.44764;18.75010;5.44764;,
 -6.21702;21.18952;0.00000;,
 -5.44764;18.75010;-5.44764;,
 0.00000;21.18952;-6.21702;,
 5.44764;18.75010;-5.44764;,
 6.21702;21.18952;0.00000;,
 5.44764;18.75010;5.44764;,
 0.00000;21.18952;6.21702;,
 0.00000;25.18760;5.11868;,
 -3.61946;25.18760;3.61946;,
 -5.11868;25.18760;0.00000;,
 -3.61946;25.18760;-3.61946;,
 0.00000;25.18760;-5.11868;,
 3.61946;25.18760;-3.61946;,
 5.11868;25.18760;0.00000;,
 3.61946;25.18760;3.61946;,
 0.00000;25.18760;5.11868;,
 0.00000;25.17065;0.00000;,
 0.00000;25.17065;0.00000;,
 0.00000;25.17065;0.00000;,
 0.00000;25.17065;0.00000;,
 0.00000;25.17065;0.00000;,
 0.00000;25.17065;0.00000;,
 0.00000;25.17065;0.00000;,
 0.00000;25.17065;0.00000;,
 0.00000;-8.22537;0.00000;,
 0.00000;-8.22537;25.15758;,
 -17.78910;-8.22537;17.78910;,
 -25.15758;-8.22537;0.00000;,
 -17.78910;-8.22537;-17.78910;,
 -0.00000;-8.22537;-25.15758;,
 17.78910;-8.22537;-17.78908;,
 25.15758;-8.22537;0.00000;,
 17.78910;-8.22537;17.78908;;
 
 56;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;1,18,19,2;,
 4;2,19,20,4;,
 4;4,20,21,6;,
 4;6,21,22,8;,
 4;8,22,23,10;,
 4;10,23,24,12;,
 4;12,24,25,14;,
 4;14,25,26,16;,
 4;18,27,28,19;,
 4;19,28,29,20;,
 4;20,29,30,21;,
 4;21,30,31,22;,
 4;22,31,32,23;,
 4;23,32,33,24;,
 4;24,33,34,25;,
 4;25,34,35,26;,
 4;27,36,37,28;,
 4;28,37,38,29;,
 4;29,38,39,30;,
 4;30,39,40,31;,
 4;31,40,41,32;,
 4;32,41,42,33;,
 4;33,42,43,34;,
 4;34,43,44,35;,
 4;36,45,46,37;,
 4;37,46,47,38;,
 4;38,47,48,39;,
 4;39,48,49,40;,
 4;40,49,50,41;,
 4;41,50,51,42;,
 4;42,51,52,43;,
 4;43,52,53,44;,
 3;45,54,46;,
 3;46,55,47;,
 3;47,56,48;,
 3;48,57,49;,
 3;49,58,50;,
 3;50,59,51;,
 3;51,60,52;,
 3;52,61,53;,
 3;62,63,64;,
 3;62,64,65;,
 3;62,65,66;,
 3;62,66,67;,
 3;62,67,68;,
 3;62,68,69;,
 3;62,69,70;,
 3;62,70,63;;
 
 MeshMaterialList {
  2;
  56;
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.367200;0.128800;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.452000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.420000;0.237300;0.000000;;
  }
 }
 MeshNormals {
  58;
  0.000000;0.605438;0.795893;,
  -0.562781;0.605438;0.562781;,
  -0.795892;0.605438;0.000000;,
  -0.562781;0.605438;-0.562781;,
  0.000000;0.605438;-0.795893;,
  0.562782;0.605437;-0.562781;,
  0.795893;0.605437;0.000000;,
  0.562782;0.605437;0.562781;,
  0.000000;0.581402;0.813616;,
  -0.575314;0.581402;0.575314;,
  -0.813616;0.581402;0.000000;,
  -0.575314;0.581402;-0.575314;,
  0.000000;0.581402;-0.813616;,
  0.575314;0.581402;-0.575314;,
  0.813616;0.581402;0.000000;,
  0.575314;0.581402;0.575314;,
  -0.000000;0.538961;0.842331;,
  -0.595618;0.538961;0.595618;,
  -0.842331;0.538961;0.000000;,
  -0.595618;0.538961;-0.595618;,
  -0.000000;0.538961;-0.842331;,
  0.595618;0.538961;-0.595618;,
  0.842330;0.538962;0.000000;,
  0.595618;0.538961;0.595618;,
  0.000000;0.484826;0.874611;,
  -0.617339;0.487632;0.617339;,
  -0.874611;0.484826;0.000000;,
  -0.617339;0.487632;-0.617339;,
  0.000000;0.484826;-0.874611;,
  0.617339;0.487632;-0.617339;,
  0.874611;0.484826;0.000000;,
  0.617339;0.487632;0.617339;,
  0.000000;0.378722;0.925511;,
  -0.651845;0.387552;0.651845;,
  -0.925511;0.378722;0.000000;,
  -0.651845;0.387552;-0.651845;,
  0.000000;0.378722;-0.925511;,
  0.651845;0.387552;-0.651845;,
  0.925511;0.378722;0.000000;,
  0.651845;0.387552;0.651845;,
  0.000000;0.307501;0.951548;,
  -0.670309;0.318390;0.670309;,
  -0.951548;0.307501;0.000000;,
  -0.670309;0.318390;-0.670309;,
  0.000000;0.307501;-0.951548;,
  0.670309;0.318390;-0.670309;,
  0.951548;0.307501;0.000000;,
  0.670309;0.318390;0.670309;,
  -0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  0.000000;0.999995;-0.003313;,
  0.002343;0.999995;-0.002343;,
  0.003313;0.999995;0.000000;,
  0.002343;0.999995;0.002343;,
  0.000000;0.999995;0.003313;,
  -0.002343;0.999995;0.002343;,
  -0.003313;0.999995;0.000000;,
  -0.002343;0.999995;-0.002343;;
  56;
  4;0,8,9,1;,
  4;1,9,10,2;,
  4;2,10,11,3;,
  4;3,11,12,4;,
  4;4,12,13,5;,
  4;5,13,14,6;,
  4;6,14,15,7;,
  4;7,15,8,0;,
  4;8,16,17,9;,
  4;9,17,18,10;,
  4;10,18,19,11;,
  4;11,19,20,12;,
  4;12,20,21,13;,
  4;13,21,22,14;,
  4;14,22,23,15;,
  4;15,23,16,8;,
  4;16,24,25,17;,
  4;17,25,26,18;,
  4;18,26,27,19;,
  4;19,27,28,20;,
  4;20,28,29,21;,
  4;21,29,30,22;,
  4;22,30,31,23;,
  4;23,31,24,16;,
  4;24,32,33,25;,
  4;25,33,34,26;,
  4;26,34,35,27;,
  4;27,35,36,28;,
  4;28,36,37,29;,
  4;29,37,38,30;,
  4;30,38,39,31;,
  4;31,39,32,24;,
  4;32,40,41,33;,
  4;33,41,42,34;,
  4;34,42,43,35;,
  4;35,43,44,36;,
  4;36,44,45,37;,
  4;37,45,46,38;,
  4;38,46,47,39;,
  4;39,47,40,32;,
  3;50,48,51;,
  3;51,48,52;,
  3;52,48,53;,
  3;53,48,54;,
  3;54,48,55;,
  3;55,48,56;,
  3;56,48,57;,
  3;57,48,50;,
  3;49,49,49;,
  3;49,49,49;,
  3;49,49,49;,
  3;49,49,49;,
  3;49,49,49;,
  3;49,49,49;,
  3;49,49,49;,
  3;49,49,49;;
 }
 MeshTextureCoords {
  71;
  0.000000;1.000000;,
  0.000000;0.833330;,
  0.125000;0.833330;,
  0.125000;1.000000;,
  0.250000;0.833330;,
  0.250000;1.000000;,
  0.375000;0.833330;,
  0.375000;1.000000;,
  0.500000;0.833330;,
  0.500000;1.000000;,
  0.625000;0.833330;,
  0.625000;1.000000;,
  0.750000;0.833330;,
  0.750000;1.000000;,
  0.875000;0.833330;,
  0.875000;1.000000;,
  1.000000;0.833330;,
  1.000000;1.000000;,
  0.000000;0.666670;,
  0.125000;0.666670;,
  0.250000;0.666670;,
  0.375000;0.666670;,
  0.500000;0.666670;,
  0.625000;0.666670;,
  0.750000;0.666670;,
  0.875000;0.666670;,
  1.000000;0.666670;,
  0.000000;0.500000;,
  0.125000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.333330;,
  0.125000;0.333330;,
  0.250000;0.333330;,
  0.375000;0.333330;,
  0.500000;0.333330;,
  0.625000;0.333330;,
  0.750000;0.333330;,
  0.875000;0.333330;,
  1.000000;0.333330;,
  0.000000;0.166670;,
  0.125000;0.166670;,
  0.250000;0.166670;,
  0.375000;0.166670;,
  0.500000;0.166670;,
  0.625000;0.166670;,
  0.750000;0.166670;,
  0.875000;0.166670;,
  1.000000;0.166670;,
  0.062500;0.000000;,
  0.125000;0.000000;,
  0.187500;0.000000;,
  0.250000;0.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
