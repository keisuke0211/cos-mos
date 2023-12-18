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
 121;
 0.00000;2.50000;2.50000;,
 -1.76777;1.76777;2.50000;,
 -1.76777;1.76777;12.50000;,
 0.00000;2.50000;12.50000;,
 -2.50000;-0.00000;2.50000;,
 -2.50000;0.00000;12.50000;,
 -1.76777;-1.76777;2.50000;,
 -1.76777;-1.76777;12.50000;,
 0.00000;-2.50000;2.50000;,
 0.00000;-2.50000;12.50000;,
 1.76777;-1.76777;2.50000;,
 1.76777;-1.76777;12.50000;,
 2.50000;-0.00000;2.50000;,
 2.50000;0.00000;12.50000;,
 1.76777;1.76777;2.50000;,
 1.76777;1.76777;12.50000;,
 0.00000;2.50000;2.50000;,
 0.00000;2.50000;12.50000;,
 0.00000;-0.00000;2.50000;,
 0.00000;-0.00000;2.50000;,
 0.00000;-0.00000;2.50000;,
 0.00000;-0.00000;2.50000;,
 0.00000;-0.00000;2.50000;,
 0.00000;-0.00000;2.50000;,
 0.00000;-0.00000;2.50000;,
 0.00000;-0.00000;2.50000;,
 0.00000;0.00000;12.50000;,
 0.00000;0.00000;12.50000;,
 0.00000;0.00000;12.50000;,
 0.00000;0.00000;12.50000;,
 0.00000;0.00000;12.50000;,
 0.00000;0.00000;12.50000;,
 0.00000;0.00000;12.50000;,
 0.00000;0.00000;12.50000;,
 0.00000;0.62500;-7.50000;,
 -0.44194;0.44194;-7.50000;,
 -0.44194;0.44194;2.50000;,
 0.00000;0.62500;2.50000;,
 -0.62500;-0.00000;-7.50000;,
 -0.62500;0.00000;2.50000;,
 -0.44194;-0.44194;-7.50000;,
 -0.44194;-0.44194;2.50000;,
 0.00000;-0.62500;-7.50000;,
 0.00000;-0.62500;2.50000;,
 0.44194;-0.44194;-7.50000;,
 0.44194;-0.44194;2.50000;,
 0.62500;-0.00000;-7.50000;,
 0.62500;0.00000;2.50000;,
 0.44194;0.44194;-7.50000;,
 0.44194;0.44194;2.50000;,
 0.00000;0.62500;-7.50000;,
 0.00000;0.62500;2.50000;,
 -0.00000;-0.00000;-7.50000;,
 -0.00000;-0.00000;-7.50000;,
 -0.00000;-0.00000;-7.50000;,
 -0.00000;-0.00000;-7.50000;,
 -0.00000;-0.00000;-7.50000;,
 -0.00000;-0.00000;-7.50000;,
 -0.00000;-0.00000;-7.50000;,
 -0.00000;-0.00000;-7.50000;,
 0.00000;0.00000;16.00000;,
 -0.67650;-0.67649;15.80970;,
 0.00000;-0.95671;15.80970;,
 0.00000;0.00000;16.00000;,
 -0.95671;0.00000;15.80970;,
 0.00000;0.00000;16.00000;,
 -0.67650;0.67650;15.80970;,
 0.00000;0.00000;16.00000;,
 0.00000;0.95671;15.80970;,
 0.00000;0.00000;16.00000;,
 0.67650;0.67650;15.80970;,
 0.00000;0.00000;16.00000;,
 0.95671;0.00000;15.80970;,
 0.00000;0.00000;16.00000;,
 0.67650;-0.67649;15.80970;,
 0.00000;0.00000;16.00000;,
 0.00000;-0.95671;15.80970;,
 -1.25000;-1.25000;15.26777;,
 0.00000;-1.76777;15.26777;,
 -1.76777;0.00000;15.26777;,
 -1.25000;1.25000;15.26777;,
 0.00000;1.76777;15.26777;,
 1.25000;1.25000;15.26777;,
 1.76777;0.00000;15.26777;,
 1.25000;-1.25000;15.26777;,
 0.00000;-1.76777;15.26777;,
 -1.63320;-1.63320;14.45671;,
 0.00000;-2.30970;14.45671;,
 -2.30970;0.00000;14.45671;,
 -1.63320;1.63320;14.45671;,
 0.00000;2.30970;14.45671;,
 1.63320;1.63320;14.45671;,
 2.30970;0.00000;14.45671;,
 1.63320;-1.63320;14.45671;,
 0.00000;-2.30970;14.45671;,
 -1.76777;-1.76777;13.50000;,
 0.00000;-2.50000;13.50000;,
 -2.50000;0.00000;13.50000;,
 -1.76777;1.76777;13.50000;,
 0.00000;2.50000;13.50000;,
 1.76777;1.76777;13.50000;,
 2.50000;-0.00000;13.50000;,
 1.76777;-1.76777;13.50000;,
 0.00000;-2.50000;13.50000;,
 0.00000;0.00000;13.50000;,
 0.00000;-2.50000;13.50000;,
 -1.76777;-1.76777;13.50000;,
 0.00000;0.00000;13.50000;,
 -2.50000;0.00000;13.50000;,
 0.00000;0.00000;13.50000;,
 -1.76777;1.76777;13.50000;,
 0.00000;0.00000;13.50000;,
 0.00000;2.50000;13.50000;,
 0.00000;0.00000;13.50000;,
 1.76777;1.76777;13.50000;,
 0.00000;0.00000;13.50000;,
 2.50000;-0.00000;13.50000;,
 0.00000;0.00000;13.50000;,
 1.76777;-1.76777;13.50000;,
 0.00000;0.00000;13.50000;,
 0.00000;-2.50000;13.50000;;
 
 80;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 3;18,1,0;,
 3;19,4,1;,
 3;20,6,4;,
 3;21,8,6;,
 3;22,10,8;,
 3;23,12,10;,
 3;24,14,12;,
 3;25,16,14;,
 3;26,3,2;,
 3;27,2,5;,
 3;28,5,7;,
 3;29,7,9;,
 3;30,9,11;,
 3;31,11,13;,
 3;32,13,15;,
 3;33,15,17;,
 4;34,35,36,37;,
 4;35,38,39,36;,
 4;38,40,41,39;,
 4;40,42,43,41;,
 4;42,44,45,43;,
 4;44,46,47,45;,
 4;46,48,49,47;,
 4;48,50,51,49;,
 3;52,35,34;,
 3;53,38,35;,
 3;54,40,38;,
 3;55,42,40;,
 3;56,44,42;,
 3;57,46,44;,
 3;58,48,46;,
 3;59,50,48;,
 3;60,61,62;,
 3;63,64,61;,
 3;65,66,64;,
 3;67,68,66;,
 3;69,70,68;,
 3;71,72,70;,
 3;73,74,72;,
 3;75,76,74;,
 4;62,61,77,78;,
 4;61,64,79,77;,
 4;64,66,80,79;,
 4;66,68,81,80;,
 4;68,70,82,81;,
 4;70,72,83,82;,
 4;72,74,84,83;,
 4;74,76,85,84;,
 4;78,77,86,87;,
 4;77,79,88,86;,
 4;79,80,89,88;,
 4;80,81,90,89;,
 4;81,82,91,90;,
 4;82,83,92,91;,
 4;83,84,93,92;,
 4;84,85,94,93;,
 4;87,86,95,96;,
 4;86,88,97,95;,
 4;88,89,98,97;,
 4;89,90,99,98;,
 4;90,91,100,99;,
 4;91,92,101,100;,
 4;92,93,102,101;,
 4;93,94,103,102;,
 3;104,105,106;,
 3;107,106,108;,
 3;109,108,110;,
 3;111,110,112;,
 3;113,112,114;,
 3;115,114,116;,
 3;117,116,118;,
 3;119,118,120;;
 
 MeshMaterialList {
  1;
  80;
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
   0.500000;0.500000;0.500000;;
  }
 }
 MeshNormals {
  51;
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;-0.000000;,
  -0.707107;0.707107;-0.000000;,
  -1.000000;-0.000000;-0.000000;,
  -0.707107;-0.707107;0.000000;,
  0.000000;-1.000000;0.000000;,
  0.707107;-0.707107;0.000000;,
  1.000000;0.000000;-0.000000;,
  0.707107;0.707107;-0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;-0.000000;,
  -0.707107;0.707107;-0.000000;,
  -1.000000;-0.000000;-0.000000;,
  -0.707107;-0.707107;0.000000;,
  0.000000;-1.000000;0.000000;,
  0.707107;-0.707107;0.000000;,
  1.000000;0.000000;-0.000000;,
  0.707107;0.707107;-0.000000;,
  -0.000000;0.000000;1.000000;,
  0.000000;-0.380611;0.924735;,
  -0.269132;-0.269132;0.924735;,
  -0.380611;0.000000;0.924735;,
  -0.269132;0.269132;0.924735;,
  0.000000;0.380611;0.924735;,
  0.269132;0.269132;0.924735;,
  0.380611;0.000000;0.924735;,
  0.269132;-0.269132;0.924735;,
  0.000000;-0.704976;0.709231;,
  -0.498494;-0.498494;0.709230;,
  -0.704977;0.000000;0.709230;,
  -0.498494;0.498494;0.709230;,
  0.000000;0.704977;0.709230;,
  0.498494;0.498494;0.709230;,
  0.704977;0.000000;0.709230;,
  0.498494;-0.498494;0.709230;,
  0.000000;-0.923104;0.384551;,
  -0.652733;-0.652733;0.384551;,
  -0.923104;0.000000;0.384551;,
  -0.652733;0.652733;0.384551;,
  0.000000;0.923104;0.384551;,
  0.652733;0.652733;0.384551;,
  0.923104;0.000000;0.384551;,
  0.652733;-0.652733;0.384551;,
  0.000000;-0.980785;0.195090;,
  -0.693520;-0.693520;0.195091;,
  -0.980785;0.000000;0.195091;,
  -0.693520;0.693520;0.195091;,
  0.000000;0.980785;0.195091;,
  0.693520;0.693520;0.195091;,
  0.980785;0.000000;0.195091;,
  0.693520;-0.693520;0.195090;;
  80;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,1,1,8;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  4;10,11,11,10;,
  4;11,12,12,11;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,15,15,14;,
  4;15,16,16,15;,
  4;16,17,17,16;,
  4;17,10,10,17;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;18,20,19;,
  3;18,21,20;,
  3;18,22,21;,
  3;18,23,22;,
  3;18,24,23;,
  3;18,25,24;,
  3;18,26,25;,
  3;18,19,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,25,33,32;,
  4;25,26,34,33;,
  4;26,19,27,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,33,41,40;,
  4;33,34,42,41;,
  4;34,27,35,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,41,49,48;,
  4;41,42,50,49;,
  4;42,35,43,50;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;;
 }
 MeshTextureCoords {
  121;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
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
  0.937500;1.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
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
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.062500;1.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.187500;1.000000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.375000;1.000000;,
  0.437500;1.000000;,
  0.500000;1.000000;,
  0.562500;1.000000;,
  0.625000;1.000000;,
  0.687500;1.000000;,
  0.750000;1.000000;,
  0.812500;1.000000;,
  0.875000;1.000000;,
  0.937500;1.000000;,
  1.000000;1.000000;;
 }
}