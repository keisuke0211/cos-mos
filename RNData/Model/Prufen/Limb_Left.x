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
 0.00000;0.00000;0.00000;,
 0.00000;1.50000;-2.50000;,
 0.00000;2.50000;0.00000;,
 -0.00000;-1.50000;-2.50000;,
 -0.00000;-2.50000;0.00000;,
 0.00000;-1.50000;2.50000;,
 0.00000;1.50000;2.50000;,
 -10.00000;0.00000;0.00000;,
 -10.00000;2.50000;0.00000;,
 -10.00000;1.50000;-2.50000;,
 -10.00000;-1.50000;-2.50000;,
 -10.00000;-2.50000;0.00000;,
 -10.00000;-1.50000;2.50000;,
 -10.00000;1.50000;2.50000;,
 0.00000;1.50000;-2.50000;,
 -2.00000;1.50000;-2.50000;,
 -2.00000;2.50000;0.00000;,
 -0.00000;-1.50000;-2.50000;,
 -2.00000;-1.50000;-2.50000;,
 -2.00000;1.50000;-2.50000;,
 0.00000;1.50000;-2.50000;,
 -2.00000;-2.50000;0.00000;,
 -2.00000;-1.50000;-2.50000;,
 -0.00000;-1.50000;-2.50000;,
 0.00000;-1.50000;2.50000;,
 -2.00000;-1.50000;2.50000;,
 0.00000;1.50000;2.50000;,
 -2.00000;1.50000;2.50000;,
 -2.00000;-1.50000;2.50000;,
 0.00000;-1.50000;2.50000;,
 -2.00000;1.50000;2.50000;,
 0.00000;1.50000;2.50000;,
 -2.00000;1.50000;-2.50000;,
 -10.00000;1.50000;-2.50000;,
 -10.00000;2.50000;0.00000;,
 -2.00000;2.50000;0.00000;,
 -2.00000;-1.50000;-2.50000;,
 -10.00000;-1.50000;-2.50000;,
 -2.00000;-2.50000;0.00000;,
 -10.00000;-2.50000;0.00000;,
 -2.00000;-1.50000;2.50000;,
 -10.00000;-1.50000;2.50000;,
 -2.00000;1.50000;2.50000;,
 -10.00000;1.50000;2.50000;;
 
 36;
 3;0,1,2;,
 3;0,3,1;,
 3;0,4,3;,
 3;0,5,4;,
 3;0,6,5;,
 3;0,2,6;,
 3;7,8,9;,
 3;7,9,10;,
 3;7,10,11;,
 3;7,11,12;,
 3;7,12,13;,
 3;7,13,8;,
 3;14,15,16;,
 3;14,16,2;,
 3;17,18,19;,
 3;17,19,20;,
 3;4,21,22;,
 3;4,22,23;,
 3;24,25,21;,
 3;24,21,4;,
 3;26,27,28;,
 3;26,28,29;,
 3;2,16,30;,
 3;2,30,31;,
 3;32,33,34;,
 3;32,34,35;,
 3;36,37,33;,
 3;36,33,32;,
 3;38,39,37;,
 3;38,37,36;,
 3;40,41,39;,
 3;40,39,38;,
 3;42,43,41;,
 3;42,41,40;,
 3;35,34,43;,
 3;35,43,42;;
 
 MeshMaterialList {
  4;
  36;
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "RNData\\Texture\\PrufenMat_1.PNG";
   }
  }
  Material {
   0.000000;0.000000;0.800000;1.000000;;
   5.000000;
   0.500000;0.500000;0.500000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "RNData\\Texture\\PrufenMat_1.PNG";
   }
  }
  Material {
   0.401600;0.401600;0.401600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "RNData\\Texture\\PrufenMat_2.PNG";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "RNData\\Texture\\PrufenMat_1.PNG";
   }
  }
 }
 MeshNormals {
  18;
  1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.928477;-0.371391;,
  -0.000000;0.000000;-1.000000;,
  0.000000;-1.000000;0.000000;,
  0.000000;-0.928477;0.371391;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.928477;-0.371391;,
  0.000000;0.991228;0.132164;,
  -0.000000;-0.991228;-0.132164;,
  -0.000000;-0.928477;-0.371391;,
  -0.000000;-0.928477;-0.371391;,
  0.000000;-0.928477;0.371391;,
  0.000000;0.928477;0.371391;,
  0.000000;0.928477;0.371391;,
  0.000000;0.991228;-0.132164;,
  0.000000;-0.991228;0.132164;;
  36;
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;8,2,1;,
  3;8,1,9;,
  3;3,3,3;,
  3;3,3,3;,
  3;10,4,11;,
  3;10,11,12;,
  3;13,5,4;,
  3;13,4,10;,
  3;6,6,6;,
  3;6,6,6;,
  3;9,1,14;,
  3;9,14,15;,
  3;2,8,16;,
  3;2,16,1;,
  3;3,3,3;,
  3;3,3,3;,
  3;4,17,12;,
  3;4,12,11;,
  3;5,13,17;,
  3;5,17,4;,
  3;6,6,6;,
  3;6,6,6;,
  3;1,16,15;,
  3;1,15,14;;
 }
 MeshTextureCoords {
  44;
  0.500000;-12.000000;,
  5.500000;-12.000000;,
  0.500000;-12.000000;,
  5.500000;-12.000000;,
  0.500000;-12.000000;,
  -4.500000;-12.000000;,
  -4.500000;-12.000000;,
  0.500000;13.000000;,
  0.500000;13.000000;,
  5.500000;13.000000;,
  5.500000;13.000000;,
  0.500000;13.000000;,
  -4.500000;13.000000;,
  -4.500000;13.000000;,
  3.000000;-12.000000;,
  3.000000;-7.000000;,
  0.500000;-7.000000;,
  2.000000;-12.000000;,
  2.000000;-7.000000;,
  -1.000000;-7.000000;,
  -1.000000;-12.000000;,
  0.500000;-7.000000;,
  3.000000;-7.000000;,
  3.000000;-12.000000;,
  -2.000000;-12.000000;,
  -2.000000;-7.000000;,
  -1.000000;-12.000000;,
  -1.000000;-7.000000;,
  2.000000;-7.000000;,
  2.000000;-12.000000;,
  -2.000000;-7.000000;,
  -2.000000;-12.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;;
 }
}
