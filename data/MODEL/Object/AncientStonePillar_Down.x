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
 45;
 -8.00000;0.00000;8.00000;,
 -8.00000;-8.00000;8.00000;,
 8.00000;-8.00000;8.00000;,
 8.00000;0.00000;8.00000;,
 -8.00000;0.00000;-8.00000;,
 -8.00000;-8.00000;-8.00000;,
 -8.00000;-8.00000;8.00000;,
 -8.00000;0.00000;8.00000;,
 8.00000;0.00000;-8.00000;,
 8.00000;-8.00000;-8.00000;,
 -8.00000;-8.00000;-8.00000;,
 -8.00000;0.00000;-8.00000;,
 8.00000;0.00000;8.00000;,
 8.00000;-8.00000;8.00000;,
 8.00000;-8.00000;-8.00000;,
 8.00000;0.00000;-8.00000;,
 -8.00000;0.00000;8.00000;,
 8.00000;0.00000;8.00000;,
 -8.00000;-8.00000;8.00000;,
 8.00000;-8.00000;8.00000;,
 0.00000;8.00000;0.00000;,
 -5.65685;8.00000;5.65685;,
 0.00000;8.00000;8.00000;,
 -8.00000;8.00000;0.00000;,
 -5.65685;8.00000;-5.65685;,
 0.00000;8.00000;-8.00000;,
 5.65685;8.00000;-5.65685;,
 8.00000;8.00000;0.00000;,
 5.65685;8.00000;5.65685;,
 -5.65685;8.00000;5.65685;,
 -5.65685;0.00000;5.65685;,
 0.00000;0.00000;8.00000;,
 0.00000;8.00000;8.00000;,
 -8.00000;8.00000;0.00000;,
 -8.00000;0.00000;0.00000;,
 -5.65685;8.00000;-5.65685;,
 -5.65685;0.00000;-5.65685;,
 0.00000;8.00000;-8.00000;,
 0.00000;0.00000;-8.00000;,
 5.65685;8.00000;-5.65685;,
 5.65685;0.00000;-5.65685;,
 8.00000;8.00000;0.00000;,
 8.00000;0.00000;0.00000;,
 5.65685;8.00000;5.65685;,
 5.65685;0.00000;5.65685;;
 
 36;
 3;0,1,2;,
 3;0,2,3;,
 3;4,5,6;,
 3;4,6,7;,
 3;8,9,10;,
 3;8,10,11;,
 3;12,13,14;,
 3;12,14,15;,
 3;4,16,17;,
 3;4,17,15;,
 3;18,5,14;,
 3;18,14,19;,
 3;20,21,22;,
 3;20,23,21;,
 3;20,24,23;,
 3;20,25,24;,
 3;20,26,25;,
 3;20,27,26;,
 3;20,28,27;,
 3;20,22,28;,
 3;29,30,31;,
 3;29,31,32;,
 3;33,34,30;,
 3;33,30,29;,
 3;35,36,34;,
 3;35,34,33;,
 3;37,38,36;,
 3;37,36,35;,
 3;39,40,38;,
 3;39,38,37;,
 3;41,42,40;,
 3;41,40,39;,
 3;43,44,42;,
 3;43,42,41;,
 3;32,31,44;,
 3;32,44,43;;
 
 MeshMaterialList {
  3;
  36;
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
  1;;
  Material {
   0.749804;0.621176;0.335686;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "Data\\Texture\\Material\\StoneTileHalf_Horizontal.PNG";
   }
  }
  Material {
   0.752941;0.621176;0.338824;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "Data\\Texture\\Material\\StoneTileHalf_Vertical.PNG";
   }
  }
  Material {
   0.517647;0.426667;0.232157;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "Data\\Texture\\Material\\Stone.PNG";
   }
  }
 }
 MeshNormals {
  23;
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.136774;0.000000;0.990602;,
  -0.797175;0.000000;0.603749;,
  -0.990602;0.000000;-0.136774;,
  -0.603749;0.000000;-0.797175;,
  0.136774;0.000000;-0.990602;,
  0.797175;0.000000;-0.603749;,
  0.990602;0.000000;0.136774;,
  0.603749;0.000000;0.797175;,
  -0.603748;0.000000;0.797175;,
  0.136774;0.000000;0.990602;,
  -0.990602;0.000000;0.136774;,
  -0.797175;0.000000;-0.603748;,
  -0.136774;0.000000;-0.990602;,
  0.603748;0.000000;-0.797175;,
  0.990602;0.000000;-0.136774;,
  0.797175;0.000000;0.603748;;
  36;
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;2,2,2;,
  3;2,2,2;,
  3;3,3,3;,
  3;3,3,3;,
  3;4,4,4;,
  3;4,4,4;,
  3;5,5,5;,
  3;5,5,5;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;15,8,7;,
  3;15,7,16;,
  3;17,9,8;,
  3;17,8,15;,
  3;18,10,9;,
  3;18,9,17;,
  3;19,11,10;,
  3;19,10,18;,
  3;20,12,11;,
  3;20,11,19;,
  3;21,13,12;,
  3;21,12,20;,
  3;22,14,13;,
  3;22,13,21;,
  3;16,7,14;,
  3;16,14,22;;
 }
 MeshTextureCoords {
  45;
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
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
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;0.500000;,
  0.676777;0.323224;,
  0.500000;0.250000;,
  0.750000;0.500000;,
  0.676777;0.676777;,
  0.500000;0.750000;,
  0.323224;0.676777;,
  0.250000;0.500000;,
  0.323224;0.323224;,
  1.984699;0.230769;,
  1.984699;1.000000;,
  0.991573;1.000000;,
  0.991573;0.230769;,
  2.396068;0.230769;,
  2.396068;1.000000;,
  1.984699;0.230769;,
  1.984699;1.000000;,
  0.991573;0.230769;,
  0.991573;1.000000;,
  -0.001553;0.230769;,
  -0.001553;1.000000;,
  -0.412921;0.230769;,
  -0.412921;1.000000;,
  -0.001553;0.230769;,
  -0.001553;1.000000;;
 }
}
