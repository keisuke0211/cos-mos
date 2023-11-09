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
 70;
 8.00000;8.00000;-8.00000;,
 -8.00000;8.00000;-8.00000;,
 -8.00000;8.00000;8.00000;,
 8.00000;8.00000;8.00000;,
 8.00000;-8.00000;-8.00000;,
 8.00000;-8.00000;8.00000;,
 -8.00000;-8.00000;8.00000;,
 -8.00000;-8.00000;-8.00000;,
 -5.00000;5.00000;-10.00000;,
 5.00000;5.00000;-10.00000;,
 5.00000;-5.00000;-10.00000;,
 -5.00000;-5.00000;-10.00000;,
 -8.00000;8.00000;-8.00000;,
 -8.00000;-8.00000;-8.00000;,
 -8.00000;-8.00000;8.00000;,
 -8.00000;8.00000;8.00000;,
 8.00000;8.00000;-8.00000;,
 8.00000;8.00000;8.00000;,
 8.00000;-8.00000;8.00000;,
 8.00000;-8.00000;-8.00000;,
 -6.50000;6.50000;-8.00000;,
 -6.50000;-6.50000;-8.00000;,
 -8.00000;-8.00000;-8.00000;,
 -8.00000;8.00000;-8.00000;,
 6.50000;6.50000;-8.00000;,
 8.00000;8.00000;-8.00000;,
 8.00000;-8.00000;-8.00000;,
 6.50000;-6.50000;-8.00000;,
 -6.50000;6.50000;-8.00000;,
 -8.00000;8.00000;-8.00000;,
 8.00000;8.00000;-8.00000;,
 6.50000;6.50000;-8.00000;,
 -6.50000;-6.50000;-8.00000;,
 6.50000;-6.50000;-8.00000;,
 8.00000;-8.00000;-8.00000;,
 -8.00000;-8.00000;-8.00000;,
 -8.00000;8.00000;8.00000;,
 -8.00000;-8.00000;8.00000;,
 8.00000;-8.00000;8.00000;,
 8.00000;-24.00000;-8.00000;,
 8.00000;-24.00000;8.00000;,
 -8.00000;-24.00000;8.00000;,
 -8.00000;-24.00000;-8.00000;,
 -5.00000;-11.00000;-10.00000;,
 5.00000;-11.00000;-10.00000;,
 5.00000;-21.00000;-10.00000;,
 -5.00000;-21.00000;-10.00000;,
 -8.00000;-8.00000;-8.00000;,
 -8.00000;-24.00000;-8.00000;,
 -8.00000;-24.00000;8.00000;,
 -8.00000;-8.00000;8.00000;,
 8.00000;-8.00000;-8.00000;,
 8.00000;-8.00000;8.00000;,
 8.00000;-24.00000;8.00000;,
 8.00000;-24.00000;-8.00000;,
 -6.50000;-9.50000;-8.00000;,
 -6.50000;-22.50000;-8.00000;,
 -8.00000;-24.00000;-8.00000;,
 -8.00000;-8.00000;-8.00000;,
 6.50000;-9.50000;-8.00000;,
 8.00000;-8.00000;-8.00000;,
 8.00000;-24.00000;-8.00000;,
 6.50000;-22.50000;-8.00000;,
 -6.50000;-9.50000;-8.00000;,
 6.50000;-9.50000;-8.00000;,
 -6.50000;-22.50000;-8.00000;,
 6.50000;-22.50000;-8.00000;,
 8.00000;-24.00000;-8.00000;,
 -8.00000;-24.00000;-8.00000;,
 -8.00000;-8.00000;8.00000;;
 
 28;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,8,11,21;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;24,27,10,9;,
 4;28,29,30,31;,
 4;28,31,9,8;,
 4;32,11,10,33;,
 4;32,33,34,35;,
 4;3,36,6,5;,
 4;34,35,37,38;,
 4;39,40,41,42;,
 4;43,44,45,46;,
 4;47,48,49,50;,
 4;51,52,53,54;,
 4;55,43,46,56;,
 4;55,56,57,58;,
 4;59,60,61,62;,
 4;59,62,45,44;,
 4;63,7,4,64;,
 4;63,64,44,43;,
 4;65,46,45,66;,
 4;65,66,67,68;,
 4;38,69,41,40;;
 
 MeshMaterialList {
  4;
  28;
  2,
  2,
  3,
  2,
  2,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  0,
  2,
  2,
  3,
  2,
  2,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.401600;0.401600;0.401600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.100000;0.100000;0.100000;;
   TextureFilename {
    "data\\TEXTURE\\Block_Nor_0.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Block_Nor_1.jpg";
   }
  }
 }
 MeshNormals {
  14;
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  -0.323381;-0.323381;-0.889297;,
  0.323381;-0.323381;-0.889297;,
  -0.323381;0.323381;-0.889297;,
  0.323381;0.323381;-0.889297;,
  -0.235702;0.235702;-0.942809;,
  0.235702;0.235702;-0.942809;,
  0.235702;-0.235702;-0.942809;,
  -0.235702;-0.235702;-0.942809;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;;
  28;
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;4,5,3,2;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;6,4,2,9;,
  4;6,9,12,12;,
  4;7,12,12,8;,
  4;7,8,3,5;,
  4;6,12,12,7;,
  4;6,7,5,4;,
  4;9,2,3,8;,
  4;9,8,12,12;,
  4;13,13,13,13;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;4,5,3,2;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;6,4,2,9;,
  4;6,9,12,12;,
  4;7,12,12,8;,
  4;7,8,3,5;,
  4;6,12,12,7;,
  4;6,7,5,4;,
  4;9,2,3,8;,
  4;9,8,12,12;,
  4;13,13,13,13;;
 }
 MeshTextureCoords {
  70;
  0.811380;1.000000;,
  0.191500;1.000000;,
  0.191500;0.000000;,
  0.811380;0.000000;,
  0.811380;0.000000;,
  0.811380;1.000000;,
  0.191500;1.000000;,
  0.191500;0.000000;,
  0.191500;0.200610;,
  0.811380;0.200610;,
  0.811380;0.811620;,
  0.191500;0.811620;,
  1.000000;0.200610;,
  1.000000;0.811620;,
  0.000000;0.811620;,
  0.000000;0.200610;,
  0.000000;0.200610;,
  1.000000;0.200610;,
  1.000000;0.811620;,
  0.000000;0.811620;,
  0.087090;0.200610;,
  0.087090;0.811620;,
  0.000000;0.811620;,
  0.000000;0.200610;,
  0.935550;0.200610;,
  1.000000;0.200610;,
  1.000000;0.811620;,
  0.935550;0.811620;,
  0.191500;0.080920;,
  0.191500;0.000000;,
  0.811380;0.000000;,
  0.811380;0.080920;,
  0.191500;0.937690;,
  0.811380;0.937690;,
  0.811380;1.000000;,
  0.191500;1.000000;,
  0.000000;0.000000;,
  0.191500;0.000000;,
  0.811380;0.000000;,
  0.811380;0.000000;,
  0.811380;1.000000;,
  0.191500;1.000000;,
  0.191500;0.000000;,
  0.191500;0.200610;,
  0.811380;0.200610;,
  0.811380;0.811620;,
  0.191500;0.811620;,
  1.000000;0.200610;,
  1.000000;0.811620;,
  0.000000;0.811620;,
  0.000000;0.200610;,
  0.000000;0.200610;,
  1.000000;0.200610;,
  1.000000;0.811620;,
  0.000000;0.811620;,
  0.087090;0.200610;,
  0.087090;0.811620;,
  0.000000;0.811620;,
  0.000000;0.200610;,
  0.935550;0.200610;,
  1.000000;0.200610;,
  1.000000;0.811620;,
  0.935550;0.811620;,
  0.191500;0.080920;,
  0.811380;0.080920;,
  0.191500;0.937690;,
  0.811380;0.937690;,
  0.811380;1.000000;,
  0.191500;1.000000;,
  0.000000;0.000000;;
 }
}
