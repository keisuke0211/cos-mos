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
 26;
 0.87923;13.47640;-0.73738;,
 1.40064;13.47640;-0.52141;,
 2.33645;-0.00009;-0.57934;,
 1.75711;-0.00009;-0.81932;,
 1.61662;13.47640;0.00000;,
 2.57642;-0.00009;0.00000;,
 1.40064;13.47640;0.52141;,
 2.33645;-0.00009;0.57934;,
 0.87923;13.47640;0.73738;,
 1.75711;-0.00009;0.81932;,
 0.35782;13.47640;0.52141;,
 1.17776;-0.00009;0.57934;,
 0.14185;13.47640;-0.00000;,
 0.93779;-0.00009;-0.00000;,
 0.35782;13.47640;-0.52141;,
 1.17776;-0.00009;-0.57934;,
 0.87923;13.47640;-0.73738;,
 1.75711;-0.00009;-0.81932;,
 0.87923;14.13885;0.00000;,
 0.87923;14.13885;0.00000;,
 0.87923;14.13885;0.00000;,
 0.87923;14.13885;0.00000;,
 0.87923;14.13885;0.00000;,
 0.87923;14.13885;0.00000;,
 0.87923;14.13885;0.00000;,
 0.87923;14.13885;0.00000;;
 
 16;
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
 3;25,16,14;;
 
 MeshMaterialList {
  2;
  16;
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
   0.511373;0.323137;0.109804;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Wood.png";
   }
  }
  Material {
   0.000000;0.614902;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  18;
  0.536210;0.844085;0.000000;,
  -0.000031;0.418583;-0.908179;,
  0.635273;0.438774;-0.635536;,
  0.894518;0.447032;0.000000;,
  0.635273;0.438774;0.635536;,
  -0.000031;0.418583;0.908179;,
  -0.648568;0.398060;0.648774;,
  -0.921040;0.389469;-0.000000;,
  -0.648568;0.398060;-0.648774;,
  -0.000058;0.006076;-0.999982;,
  0.705900;0.052054;-0.706396;,
  0.997473;0.071041;0.000000;,
  0.705900;0.052054;0.706396;,
  -0.000058;0.006076;0.999982;,
  -0.706352;-0.039938;0.706733;,
  -0.998260;-0.058959;-0.000000;,
  -0.706352;-0.039938;-0.706733;,
  -0.536210;0.844085;-0.000000;;
  16;
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,8,16,15;,
  4;8,1,9,16;,
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;17,6,5;,
  3;17,7,6;,
  3;17,8,7;,
  3;17,1,8;;
 }
 MeshTextureCoords {
  26;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;0.333333;,
  0.000000;0.333333;,
  0.250000;0.000000;,
  0.250000;0.333333;,
  0.375000;0.000000;,
  0.375000;0.333333;,
  0.500000;0.000000;,
  0.500000;0.333333;,
  0.625000;0.000000;,
  0.625000;0.333333;,
  0.750000;0.000000;,
  0.750000;0.333333;,
  0.875000;0.000000;,
  0.875000;0.333333;,
  1.000000;0.000000;,
  1.000000;0.333333;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;;
 }
}
