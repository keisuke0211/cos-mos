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
 61;
 0.00000;12.58029;3.50896;,
 -2.48121;12.58029;2.48121;,
 -2.11520;5.89525;2.11520;,
 0.00000;5.89525;2.99133;,
 -3.50896;12.58029;0.00000;,
 -2.99133;5.89525;0.00000;,
 -2.48121;12.58029;-2.48121;,
 -2.11520;5.89525;-2.11520;,
 0.00000;12.58029;-3.50896;,
 0.00000;5.89525;-2.99133;,
 2.48121;12.58029;-2.48121;,
 2.11520;5.89525;-2.11519;,
 3.50896;12.58029;0.00000;,
 2.99133;5.89525;0.00000;,
 2.48121;12.58029;2.48121;,
 2.11520;5.89525;2.11519;,
 0.00000;12.58029;3.50896;,
 0.00000;5.89525;2.99133;,
 -2.10467;0.00000;2.10467;,
 0.00000;0.00000;2.97645;,
 -2.97645;0.00000;0.00000;,
 -2.10467;0.00000;-2.10467;,
 0.00000;0.00000;-2.97645;,
 2.10467;0.00000;-2.10467;,
 2.97645;0.00000;0.00000;,
 2.10467;0.00000;2.10467;,
 0.00000;0.00000;2.97645;,
 -2.32544;-5.89525;2.32544;,
 0.00000;-5.89525;3.28867;,
 -3.28867;-5.89525;0.00000;,
 -2.32544;-5.89525;-2.32544;,
 -0.00000;-5.89525;-3.28867;,
 2.32544;-5.89525;-2.32544;,
 3.28867;-5.89525;0.00000;,
 2.32544;-5.89525;2.32544;,
 0.00000;-5.89525;3.28867;,
 -3.09009;-12.58029;3.09009;,
 0.00000;-12.58029;4.37004;,
 -4.37004;-12.58029;0.00000;,
 -3.09009;-12.58029;-3.09009;,
 -0.00000;-12.58029;-4.37004;,
 3.09009;-12.58029;-3.09009;,
 4.37004;-12.58029;0.00000;,
 3.09009;-12.58029;3.09009;,
 0.00000;-12.58029;4.37004;,
 0.00000;12.58029;0.00000;,
 0.00000;12.58029;0.00000;,
 0.00000;12.58029;0.00000;,
 0.00000;12.58029;0.00000;,
 0.00000;12.58029;0.00000;,
 0.00000;12.58029;0.00000;,
 0.00000;12.58029;0.00000;,
 0.00000;12.58029;0.00000;,
 0.00000;-12.58029;0.00000;,
 0.00000;-12.58029;0.00000;,
 0.00000;-12.58029;0.00000;,
 0.00000;-12.58029;0.00000;,
 0.00000;-12.58029;0.00000;,
 0.00000;-12.58029;0.00000;,
 0.00000;-12.58029;0.00000;,
 0.00000;-12.58029;0.00000;;
 
 48;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;3,2,18,19;,
 4;2,5,20,18;,
 4;5,7,21,20;,
 4;7,9,22,21;,
 4;9,11,23,22;,
 4;11,13,24,23;,
 4;13,15,25,24;,
 4;15,17,26,25;,
 4;19,18,27,28;,
 4;18,20,29,27;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;25,26,35,34;,
 4;28,27,36,37;,
 4;27,29,38,36;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;34,35,44,43;,
 3;45,1,0;,
 3;46,4,1;,
 3;47,6,4;,
 3;48,8,6;,
 3;49,10,8;,
 3;50,12,10;,
 3;51,14,12;,
 3;52,16,14;,
 3;53,37,36;,
 3;54,36,38;,
 3;55,38,39;,
 3;56,39,40;,
 3;57,40,41;,
 3;58,41,42;,
 3;59,42,43;,
 3;60,43,44;;
 
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\OneDrive\\�f�X�N�g�b�v\\�摜\\���Ɛ���\\Wood.jpg";
   }
  }
 }
 MeshNormals {
  42;
  0.000000;1.000000;0.000000;,
  0.000000;-0.077200;0.997016;,
  -0.704997;-0.077200;0.704997;,
  -0.997016;-0.077200;0.000000;,
  -0.704997;-0.077200;-0.704997;,
  0.000000;-0.077200;-0.997016;,
  0.704997;-0.077200;-0.704996;,
  0.997016;-0.077200;0.000000;,
  0.704997;-0.077200;0.704996;,
  0.000001;-0.039898;0.999204;,
  -0.706544;-0.039898;0.706544;,
  -0.999204;-0.039898;0.000000;,
  -0.706544;-0.039898;-0.706544;,
  0.000001;-0.039898;-0.999204;,
  0.706544;-0.039898;-0.706543;,
  0.999204;-0.039898;0.000000;,
  0.706544;-0.039898;0.706543;,
  0.000001;0.025194;0.999683;,
  -0.706882;0.025193;0.706882;,
  -0.999683;0.025193;0.000000;,
  -0.706882;0.025193;-0.706882;,
  0.000001;0.025194;-0.999683;,
  0.706883;0.025194;-0.706882;,
  0.999683;0.025194;0.000000;,
  0.706883;0.025194;0.706882;,
  0.000000;0.106484;0.994314;,
  -0.703087;0.106483;0.703087;,
  -0.994314;0.106483;0.000000;,
  -0.703087;0.106483;-0.703087;,
  0.000000;0.106484;-0.994314;,
  0.703087;0.106484;-0.703086;,
  0.994314;0.106484;0.000000;,
  0.703087;0.106484;0.703086;,
  0.000000;0.159684;0.987168;,
  -0.698033;0.159684;0.698033;,
  -0.987168;0.159684;0.000000;,
  -0.698033;0.159684;-0.698033;,
  0.000000;0.159684;-0.987168;,
  0.698033;0.159684;-0.698033;,
  0.987168;0.159684;0.000000;,
  0.698033;0.159684;0.698033;,
  0.000000;-1.000000;0.000000;;
  48;
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,8,16,15;,
  4;8,1,9,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,17,25,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,25,33,40;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;41,41,41;,
  3;41,41,41;,
  3;41,41,41;,
  3;41,41,41;,
  3;41,41,41;,
  3;41,41,41;,
  3;41,41,41;,
  3;41,41,41;;
 }
 MeshTextureCoords {
  61;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.000000;,
  0.250000;0.250000;,
  0.375000;0.000000;,
  0.375000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.625000;0.000000;,
  0.625000;0.250000;,
  0.750000;0.000000;,
  0.750000;0.250000;,
  0.875000;0.000000;,
  0.875000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.750000;1.000000;,
  0.875000;1.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
}