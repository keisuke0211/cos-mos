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
 18;
 0.00000;0.71638;-0.18757;,
 -4.02456;0.71638;-0.04408;,
 -4.02456;0.03649;-0.04407;,
 0.00000;0.03649;-0.18757;,
 -11.44151;0.71639;-7.65809;,
 -11.44151;0.03649;-7.65809;,
 -9.92607;0.71638;-17.58417;,
 -9.92607;0.03649;-17.58417;,
 0.00000;0.71639;-21.62489;,
 0.00000;0.03650;-21.62490;,
 9.92607;0.71639;-17.58416;,
 9.92607;0.03650;-17.58417;,
 11.44151;0.71639;-7.65809;,
 11.44151;0.03650;-7.65809;,
 4.02456;0.71638;-0.04408;,
 4.02456;0.03649;-0.04407;,
 -0.00000;3.39814;-11.18772;,
 -0.00000;2.71825;-11.18772;;
 
 24;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,0,3,15;,
 3;16,1,0;,
 3;16,4,1;,
 3;16,6,4;,
 3;16,8,6;,
 3;16,10,8;,
 3;16,12,10;,
 3;16,14,12;,
 3;16,0,14;,
 3;17,3,2;,
 3;17,2,5;,
 3;17,5,7;,
 3;17,7,9;,
 3;17,9,11;,
 3;17,11,13;,
 3;17,13,15;,
 3;17,15,3;;
 
 MeshMaterialList {
  2;
  24;
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.542745;0.787451;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.470000;0.318863;0.462627;;
   TextureFilename {
    "data\\TEXTURE\\Pearl.jpg";
   }
  }
  Material {
   0.367059;0.762353;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\kai.jpg";
   }
  }
 }
 MeshNormals {
  26;
  -0.000000;0.999496;0.031742;,
  0.000000;0.000004;1.000000;,
  -0.372252;0.000003;0.928132;,
  -0.952214;0.000001;0.305433;,
  -0.785154;0.000001;-0.619300;,
  0.000000;0.000003;-1.000000;,
  0.785154;0.000003;-0.619300;,
  0.952214;0.000001;0.305433;,
  0.372252;0.000003;0.928132;,
  0.000000;-0.999496;-0.031742;,
  -0.083566;0.975287;0.204527;,
  0.000000;0.971545;0.236856;,
  -0.207958;0.975819;0.067303;,
  -0.171110;0.974796;-0.143161;,
  -0.000000;0.968540;-0.248859;,
  0.171109;0.974796;-0.143161;,
  0.207958;0.975820;0.067303;,
  0.083566;0.975287;0.204527;,
  0.000000;-0.971545;-0.236856;,
  0.083566;-0.975287;-0.204527;,
  0.207959;-0.975819;-0.067303;,
  0.171110;-0.974796;0.143161;,
  0.000000;-0.968540;0.248859;,
  -0.171110;-0.974796;0.143161;,
  -0.207958;-0.975819;-0.067303;,
  -0.083566;-0.975287;-0.204527;;
  24;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,1,1,8;,
  3;0,10,11;,
  3;0,12,10;,
  3;0,13,12;,
  3;0,14,13;,
  3;0,15,14;,
  3;0,16,15;,
  3;0,17,16;,
  3;0,11,17;,
  3;9,18,19;,
  3;9,19,20;,
  3;9,20,21;,
  3;9,21,22;,
  3;9,22,23;,
  3;9,23,24;,
  3;9,24,25;,
  3;9,25,18;;
 }
 MeshTextureCoords {
  18;
  0.020778;0.504182;,
  0.016362;0.668241;,
  0.016604;0.668239;,
  0.021020;0.504180;,
  0.329273;0.967787;,
  0.329515;0.967785;,
  0.733223;0.902500;,
  0.733465;0.902498;,
  0.894353;0.496558;,
  0.894595;0.496556;,
  0.726163;0.093491;,
  0.726405;0.093489;,
  0.321135;0.035264;,
  0.321376;0.035262;,
  0.013499;0.340225;,
  0.013741;0.340223;,
  0.468520;0.500275;,
  0.468762;0.500273;;
 }
}
