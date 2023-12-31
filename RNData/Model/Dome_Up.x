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
 44;
 0.00000;5.00000;0.00000;,
 -1.35299;4.61940;1.35299;,
 0.00000;4.61940;1.91342;,
 0.00000;5.00000;0.00000;,
 -1.91342;4.61940;-0.00000;,
 0.00000;5.00000;0.00000;,
 -1.35299;4.61940;-1.35299;,
 0.00000;5.00000;0.00000;,
 0.00000;4.61940;-1.91342;,
 0.00000;5.00000;0.00000;,
 1.35299;4.61940;-1.35299;,
 0.00000;5.00000;0.00000;,
 1.91342;4.61940;0.00000;,
 0.00000;5.00000;0.00000;,
 1.35299;4.61940;1.35299;,
 0.00000;5.00000;0.00000;,
 0.00000;4.61940;1.91342;,
 -2.50000;3.53553;2.50000;,
 0.00000;3.53553;3.53553;,
 -3.53553;3.53553;-0.00000;,
 -2.50000;3.53553;-2.50000;,
 0.00000;3.53553;-3.53553;,
 2.50000;3.53553;-2.50000;,
 3.53553;3.53553;0.00000;,
 2.50000;3.53553;2.50000;,
 0.00000;3.53553;3.53553;,
 -3.26641;1.91342;3.26641;,
 0.00000;1.91342;4.61940;,
 -4.61940;1.91342;-0.00000;,
 -3.26641;1.91342;-3.26641;,
 0.00000;1.91342;-4.61940;,
 3.26641;1.91342;-3.26641;,
 4.61940;1.91342;0.00000;,
 3.26641;1.91342;3.26641;,
 -3.53553;-0.00000;3.53553;,
 0.00000;-0.00000;5.00000;,
 -5.00000;-0.00000;-0.00000;,
 -3.53553;-0.00000;-3.53553;,
 0.00000;-0.00000;-5.00000;,
 3.53553;-0.00000;-3.53553;,
 5.00000;-0.00000;0.00000;,
 3.53553;-0.00000;3.53553;,
 0.00000;1.91342;4.61940;,
 0.00000;-0.00000;5.00000;;
 
 56;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 3;1,17,18;,
 3;1,18,2;,
 3;4,19,17;,
 3;4,17,1;,
 3;6,20,19;,
 3;6,19,4;,
 3;8,21,20;,
 3;8,20,6;,
 3;10,22,21;,
 3;10,21,8;,
 3;12,23,22;,
 3;12,22,10;,
 3;14,24,23;,
 3;14,23,12;,
 3;16,25,24;,
 3;16,24,14;,
 3;17,26,27;,
 3;17,27,18;,
 3;19,28,26;,
 3;19,26,17;,
 3;20,29,28;,
 3;20,28,19;,
 3;21,30,29;,
 3;21,29,20;,
 3;22,31,30;,
 3;22,30,21;,
 3;23,32,31;,
 3;23,31,22;,
 3;24,33,32;,
 3;24,32,23;,
 3;26,34,35;,
 3;26,35,27;,
 3;28,36,34;,
 3;28,34,26;,
 3;29,37,36;,
 3;29,36,28;,
 3;30,38,37;,
 3;30,37,29;,
 3;31,39,38;,
 3;31,38,30;,
 3;32,40,39;,
 3;32,39,31;,
 3;33,41,40;,
 3;33,40,32;,
 3;42,43,41;,
 3;42,41,33;,
 3;25,42,33;,
 3;25,33,24;;
 
 MeshMaterialList {
  1;
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
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  33;
  0.000000;1.000000;-0.000000;,
  0.046420;0.907858;0.416700;,
  -0.261828;0.907858;0.327475;,
  -0.416700;0.907858;0.046419;,
  -0.327475;0.907858;-0.261828;,
  -0.046419;0.907858;-0.416700;,
  0.261828;0.907858;-0.327475;,
  0.416700;0.907858;-0.046419;,
  0.327475;0.907858;0.261827;,
  0.017941;0.709116;0.704863;,
  -0.485728;0.709116;0.511100;,
  -0.704863;0.709116;0.017941;,
  -0.511100;0.709116;-0.485728;,
  -0.017941;0.709116;-0.704863;,
  0.485728;0.709116;-0.511100;,
  0.704863;0.709116;-0.017941;,
  0.511100;0.709116;0.485728;,
  0.009213;0.384535;0.923065;,
  -0.646191;0.384535;0.659220;,
  -0.923065;0.384535;0.009213;,
  -0.659220;0.384535;-0.646191;,
  -0.009213;0.384535;-0.923065;,
  0.646191;0.384535;-0.659220;,
  0.923065;0.384535;-0.009213;,
  0.659220;0.384535;0.646191;,
  -0.134193;0.193326;0.971914;,
  -0.782136;0.193326;0.592358;,
  -0.971914;0.193326;-0.134193;,
  -0.592358;0.193326;-0.782136;,
  0.134193;0.193326;-0.971914;,
  0.782136;0.193326;-0.592358;,
  0.971914;0.193326;0.134193;,
  0.592358;0.193326;0.782136;;
  56;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,1,8;,
  3;2,10,9;,
  3;2,9,1;,
  3;3,11,10;,
  3;3,10,2;,
  3;4,12,11;,
  3;4,11,3;,
  3;5,13,12;,
  3;5,12,4;,
  3;6,14,13;,
  3;6,13,5;,
  3;7,15,14;,
  3;7,14,6;,
  3;8,16,15;,
  3;8,15,7;,
  3;1,9,16;,
  3;1,16,8;,
  3;10,18,17;,
  3;10,17,9;,
  3;11,19,18;,
  3;11,18,10;,
  3;12,20,19;,
  3;12,19,11;,
  3;13,21,20;,
  3;13,20,12;,
  3;14,22,21;,
  3;14,21,13;,
  3;15,23,22;,
  3;15,22,14;,
  3;16,24,23;,
  3;16,23,15;,
  3;18,26,25;,
  3;18,25,17;,
  3;19,27,26;,
  3;19,26,18;,
  3;20,28,27;,
  3;20,27,19;,
  3;21,29,28;,
  3;21,28,20;,
  3;22,30,29;,
  3;22,29,21;,
  3;23,31,30;,
  3;23,30,22;,
  3;24,32,31;,
  3;24,31,23;,
  3;17,25,32;,
  3;17,32,24;,
  3;9,17,24;,
  3;9,24,16;;
 }
 MeshTextureCoords {
  44;
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.375000;,
  1.000000;0.500000;;
 }
}
