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
 8;
 6.00000;22.00000;8.00000;,
 -6.00000;22.00000;8.00000;,
 -6.00000;-22.00000;8.00000;,
 6.00000;-22.00000;8.00000;,
 7.50729;23.54951;6.00000;,
 -7.67623;23.68472;6.00000;,
 -7.60314;-23.76945;6.00000;,
 7.68904;-23.76971;6.00000;;
 
 5;
 4;0,1,2,3;,
 4;4,5,1,0;,
 4;3,2,6,7;,
 4;2,1,5,6;,
 4;0,3,7,4;;
 
 MeshMaterialList {
  2;
  5;
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
    "C:\\Users\\student\\Documents\\Project\\data\\TEXTURE\\Crater.png";
   }
  }
  Material {
   0.454902;0.454902;0.454902;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Documents\\Project\\data\\TEXTURE\\Crater.png";
   }
  }
 }
 MeshNormals {
  8;
  0.003419;0.779195;0.626772;,
  0.313301;0.311318;0.897175;,
  -0.306395;0.309932;0.900036;,
  0.309325;-0.295583;0.903852;,
  -0.304769;-0.295467;0.905437;,
  -0.000006;-0.748934;0.662644;,
  -0.773127;-0.000600;0.634251;,
  0.782221;0.001470;0.622999;;
  5;
  4;1,2,4,3;,
  4;0,0,2,1;,
  4;3,4,5,5;,
  4;4,2,6,6;,
  4;1,3,7,7;;
 }
 MeshTextureCoords {
  8;
  0.200000;0.092593;,
  0.800000;0.092593;,
  0.800000;0.907407;,
  0.200000;0.907407;,
  0.124636;0.063898;,
  0.883811;0.061394;,
  0.880157;0.940175;,
  0.115548;0.940180;;
 }
}
