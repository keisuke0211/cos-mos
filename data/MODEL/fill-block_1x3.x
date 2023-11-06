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
 8.18466;23.54951;6.00000;,
 -8.01475;23.68472;6.00000;,
 -8.03199;-24.11132;6.00000;,
 8.28923;-24.11229;6.00000;;
 
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
    "C:\\Users\\student\\Documents\\Project\\data\\TEXTURE\\Crater.png";
   }
  }
  Material {
   0.455200;0.455200;0.455200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Documents\\Project\\data\\TEXTURE\\Crater.png";
   }
  }
 }
 MeshNormals {
  9;
  0.003285;0.776035;0.630682;,
  0.259020;0.300160;0.918048;,
  -0.272878;0.302666;0.913198;,
  0.251683;-0.259202;0.932454;,
  -0.267524;-0.261601;0.927360;,
  0.379998;-0.391350;0.838121;,
  -0.403741;-0.394802;0.825303;,
  -0.703058;0.000127;0.711133;,
  0.667008;0.000723;0.745050;;
  5;
  4;1,2,4,3;,
  4;0,0,2,1;,
  4;3,4,6,5;,
  4;4,2,7,6;,
  4;1,3,5,8;;
 }
 MeshTextureCoords {
  8;
  0.200000;-0.600000;,
  0.800000;-0.600000;,
  0.800000;1.600000;,
  0.200000;1.600000;,
  0.090767;-0.677475;,
  0.900738;-0.684236;,
  0.901600;1.705566;,
  0.085539;1.705614;;
 }
}
