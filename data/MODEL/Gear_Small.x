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
 232;
 -6.40313;0.01514;-4.48698;,
 -5.91596;-2.43404;-4.48698;,
 -5.91596;-2.43404;4.38302;,
 -6.40313;0.01514;4.38302;,
 -4.73340;-1.94420;-4.48698;,
 -5.12313;0.01514;-4.48698;,
 -5.12313;0.01514;4.38302;,
 -4.73339;-1.94420;4.38302;,
 -3.62352;-3.60525;-4.48698;,
 -3.62352;-3.60525;4.38302;,
 -4.52862;-4.51035;-4.48698;,
 -2.54817;-5.87980;-4.43430;,
 -2.45231;-5.89769;4.38302;,
 -4.52861;-4.51035;4.38302;,
 -1.96247;-4.71512;-4.48698;,
 -1.96247;-4.71512;4.38302;,
 -0.00313;-5.10486;-4.48698;,
 -0.00313;-5.10486;4.38302;,
 -0.00313;-6.38486;-4.48698;,
 2.44604;-5.89769;-4.48698;,
 2.44604;-5.89769;4.38302;,
 -0.00313;-6.38486;4.38302;,
 1.95621;-4.71512;-4.48698;,
 -0.00313;-5.10486;-4.48698;,
 -0.00313;-5.10486;4.38302;,
 1.95621;-4.71512;4.38302;,
 3.61725;-3.60525;-4.48698;,
 3.61726;-3.60525;4.38302;,
 4.52235;-4.51035;-4.48698;,
 5.90970;-2.43404;-4.48698;,
 5.90970;-2.43404;4.38302;,
 4.52235;-4.51035;4.38302;,
 4.72713;-1.94420;-4.48698;,
 4.72713;-1.94420;4.38302;,
 5.11687;0.01514;-4.48698;,
 5.11687;0.01514;4.38302;,
 6.39687;0.01514;-4.48698;,
 5.90970;2.46431;-4.48698;,
 5.90970;2.46431;4.38302;,
 6.39687;0.01514;4.38302;,
 4.72713;1.97448;-4.48698;,
 4.72713;1.97448;4.38302;,
 3.61725;3.63552;-4.48698;,
 3.61726;3.63552;4.38302;,
 4.52235;4.54062;-4.48698;,
 2.44604;5.92797;-4.48698;,
 2.44604;5.92797;4.38302;,
 4.52235;4.54062;4.38302;,
 1.95621;4.74540;-4.48698;,
 1.95621;4.74540;4.38302;,
 -0.00313;5.13514;-4.48698;,
 -0.00313;5.13514;4.38302;,
 -0.00313;6.41514;-4.48698;,
 -2.45231;5.92797;-4.48698;,
 -2.45231;5.92797;4.38302;,
 -0.01658;6.43577;4.42741;,
 -1.96247;4.74540;-4.48698;,
 -1.96247;4.74540;4.38302;,
 -3.62352;3.63552;-4.48698;,
 -3.62352;3.63552;4.38302;,
 -4.52862;4.54062;-4.48698;,
 -5.91596;2.46431;-4.48698;,
 -5.91596;2.46431;4.38302;,
 -4.52862;4.54062;4.38302;,
 -4.73340;1.97447;-4.48698;,
 -4.73339;1.97447;4.38302;,
 -6.40313;0.01514;-4.48698;,
 -5.12313;0.01514;-4.48698;,
 -4.73340;-1.94420;-4.48698;,
 -5.91596;-2.43404;-4.48698;,
 -6.40313;0.01514;4.38302;,
 -5.91596;-2.43404;4.38302;,
 -4.73339;-1.94420;4.38302;,
 -5.12313;0.01514;4.38302;,
 -3.62352;-3.60525;-4.48698;,
 -4.52862;-4.51035;-4.48698;,
 -4.52861;-4.51035;4.38302;,
 -3.62352;-3.60525;4.38302;,
 -1.96247;-4.71512;-4.48698;,
 -2.54817;-5.87980;-4.43430;,
 -2.45231;-5.89769;4.38302;,
 -1.96247;-4.71512;4.38302;,
 -0.00313;-5.10486;-4.48698;,
 -0.00313;-6.38486;-4.48698;,
 -0.00313;-6.38486;4.38302;,
 -0.00313;-5.10486;4.38302;,
 1.95621;-4.71512;-4.48698;,
 2.44604;-5.89769;-4.48698;,
 2.44604;-5.89769;4.38302;,
 1.95621;-4.71512;4.38302;,
 3.61725;-3.60525;-4.48698;,
 4.52235;-4.51035;-4.48698;,
 4.52235;-4.51035;4.38302;,
 3.61726;-3.60525;4.38302;,
 4.72713;-1.94420;-4.48698;,
 5.90970;-2.43404;-4.48698;,
 5.90970;-2.43404;4.38302;,
 4.72713;-1.94420;4.38302;,
 5.11687;0.01514;-4.48698;,
 6.39687;0.01514;-4.48698;,
 6.39687;0.01514;4.38302;,
 5.11687;0.01514;4.38302;,
 4.72713;1.97448;-4.48698;,
 5.90970;2.46431;-4.48698;,
 5.90970;2.46431;4.38302;,
 4.72713;1.97448;4.38302;,
 3.61725;3.63552;-4.48698;,
 4.52235;4.54062;-4.48698;,
 4.52235;4.54062;4.38302;,
 3.61726;3.63552;4.38302;,
 1.95621;4.74540;-4.48698;,
 2.44604;5.92797;-4.48698;,
 2.44604;5.92797;4.38302;,
 1.95621;4.74540;4.38302;,
 -0.00313;5.13514;-4.48698;,
 -0.00313;6.41514;-4.48698;,
 -0.01658;6.43577;4.42741;,
 -0.00313;5.13514;4.38302;,
 -1.96247;4.74540;-4.48698;,
 -2.45231;5.92797;-4.48698;,
 -2.45231;5.92797;4.38302;,
 -1.96247;4.74540;4.38302;,
 -3.62352;3.63552;-4.48698;,
 -4.52862;4.54062;-4.48698;,
 -4.52862;4.54062;4.38302;,
 -3.62352;3.63552;4.38302;,
 -4.73340;1.97447;-4.48698;,
 -5.91596;2.46431;-4.48698;,
 -5.91596;2.46431;4.38302;,
 -4.73339;1.97447;4.38302;,
 -5.44670;5.81450;4.38302;,
 -5.44670;5.81450;-4.48698;,
 -5.91596;-2.43404;-4.48698;,
 -6.40313;0.01514;-4.48698;,
 -6.40313;0.01514;4.38302;,
 -5.91596;-2.43404;4.38302;,
 -7.25528;-3.25419;-4.48698;,
 -5.79887;-5.43385;-4.48698;,
 -5.79887;-5.43385;4.38302;,
 -7.25528;-3.25419;4.38302;,
 -4.52862;-4.51035;-4.48698;,
 -4.52861;-4.51035;4.38302;,
 -2.54817;-5.87980;-4.43430;,
 -2.45231;-5.89769;4.38302;,
 -2.79402;-7.42972;-4.48698;,
 -0.27371;-7.93104;-4.48698;,
 -0.27371;-7.93104;4.38302;,
 -2.79402;-7.42972;4.38302;,
 2.44604;-5.89769;-4.48698;,
 -0.00313;-6.38486;-4.48698;,
 -0.00313;-6.38486;4.38302;,
 2.44604;-5.89769;4.38302;,
 3.39396;-7.15164;-4.48698;,
 5.31810;-5.86597;-4.48698;,
 5.31811;-5.86597;4.38302;,
 3.39396;-7.15164;4.38302;,
 4.52235;-4.51035;-4.48698;,
 4.52235;-4.51035;4.38302;,
 5.90970;-2.43404;-4.48698;,
 5.90970;-2.43404;4.38302;,
 7.46439;-2.66182;-4.48698;,
 7.92038;-0.36938;-4.48698;,
 7.92038;-0.36938;4.38302;,
 7.46439;-2.66182;4.38302;,
 6.39687;0.01514;-4.48698;,
 6.39687;0.01514;4.38302;,
 5.90970;2.46431;-4.48698;,
 5.90970;2.46431;4.38302;,
 7.17334;3.39772;-4.48698;,
 5.86829;5.35087;-4.48698;,
 5.86829;5.35087;4.38302;,
 7.17334;3.39772;4.38302;,
 4.52235;4.54062;-4.48698;,
 4.52235;4.54062;4.38302;,
 2.44604;5.92797;-4.48698;,
 2.44604;5.92797;4.38302;,
 2.76287;7.40617;-4.48698;,
 0.29234;8.01514;-4.48698;,
 0.29234;8.01514;4.38302;,
 2.76287;7.40617;4.38302;,
 -0.00313;6.41514;-4.48698;,
 -0.01658;6.43577;4.42741;,
 -2.45231;5.92797;-4.48698;,
 -2.45231;5.92797;4.38302;,
 -3.27789;7.26366;-4.48698;,
 -3.27788;7.26366;4.38302;,
 -4.52862;4.54062;-4.48698;,
 -4.52862;4.54062;4.38302;,
 -5.91596;2.46431;-4.48698;,
 -5.91596;2.46431;4.38302;,
 -7.43253;2.88374;-4.48698;,
 -7.96477;0.20800;-4.48698;,
 -7.96477;0.20800;4.38302;,
 -7.43253;2.88374;4.38302;,
 -6.40313;0.01514;-4.48698;,
 -6.40313;0.01514;4.38302;,
 -7.25528;-3.25419;-4.48698;,
 -5.79887;-5.43385;-4.48698;,
 -7.25528;-3.25419;4.38302;,
 -5.79887;-5.43385;4.38302;,
 -2.79402;-7.42972;-4.48698;,
 -0.27371;-7.93104;-4.48698;,
 -2.79402;-7.42972;4.38302;,
 -0.27371;-7.93104;4.38302;,
 -0.00313;-6.38486;4.38302;,
 -2.45231;-5.89769;4.38302;,
 3.39396;-7.15164;-4.48698;,
 5.31810;-5.86597;-4.48698;,
 3.39396;-7.15164;4.38302;,
 5.31811;-5.86597;4.38302;,
 7.46439;-2.66182;-4.48698;,
 7.92038;-0.36938;-4.48698;,
 7.46439;-2.66182;4.38302;,
 7.92038;-0.36938;4.38302;,
 7.17334;3.39772;-4.48698;,
 5.86829;5.35087;-4.48698;,
 7.17334;3.39772;4.38302;,
 5.86829;5.35087;4.38302;,
 2.76287;7.40617;-4.48698;,
 0.29234;8.01514;-4.48698;,
 2.76287;7.40617;4.38302;,
 0.29234;8.01514;4.38302;,
 -3.27789;7.26366;-4.48698;,
 -5.44670;5.81450;-4.48698;,
 -3.27788;7.26366;4.38302;,
 -5.44670;5.81450;4.38302;,
 -7.43253;2.88374;-4.48698;,
 -7.96477;0.20800;-4.48698;,
 -7.43253;2.88374;4.38302;,
 -7.96477;0.20800;4.38302;,
 2.44604;-5.89769;4.38302;,
 2.44604;-5.89769;-4.48698;;
 
 111;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,4,7,9;,
 4;10,11,12,13;,
 4;14,8,9,15;,
 4;16,14,15,17;,
 4;18,19,20,21;,
 4;22,23,24,25;,
 4;26,22,25,27;,
 4;28,29,30,31;,
 4;32,26,27,33;,
 4;34,32,33,35;,
 4;36,37,38,39;,
 4;40,34,35,41;,
 4;42,40,41,43;,
 4;44,45,46,47;,
 4;48,42,43,49;,
 4;50,48,49,51;,
 4;52,53,54,55;,
 4;56,50,51,57;,
 4;58,56,57,59;,
 4;60,61,62,63;,
 4;64,58,59,65;,
 4;5,64,65,6;,
 4;66,67,68,69;,
 4;70,71,72,73;,
 4;69,68,74,75;,
 4;71,76,77,72;,
 4;75,74,78,79;,
 4;76,80,81,77;,
 4;79,78,82,83;,
 4;80,84,85,81;,
 4;83,82,86,87;,
 4;84,88,89,85;,
 4;87,86,90,91;,
 4;88,92,93,89;,
 4;91,90,94,95;,
 4;92,96,97,93;,
 4;95,94,98,99;,
 4;96,100,101,97;,
 4;99,98,102,103;,
 4;100,104,105,101;,
 4;103,102,106,107;,
 4;104,108,109,105;,
 4;107,106,110,111;,
 4;108,112,113,109;,
 4;111,110,114,115;,
 4;112,116,117,113;,
 4;115,114,118,119;,
 4;116,120,121,117;,
 4;119,118,122,123;,
 4;120,124,125,121;,
 4;123,122,126,127;,
 4;124,128,129,125;,
 4;127,126,67,66;,
 4;128,70,73,129;,
 4;63,130,131,60;,
 4;132,133,134,135;,
 4;136,137,138,139;,
 4;140,132,135,141;,
 4;142,140,141,143;,
 4;144,145,146,147;,
 4;148,149,150,151;,
 4;152,153,154,155;,
 4;156,148,151,157;,
 4;158,156,157,159;,
 4;160,161,162,163;,
 4;164,158,159,165;,
 4;166,164,165,167;,
 4;168,169,170,171;,
 4;172,166,167,173;,
 4;174,172,173,175;,
 4;176,177,178,179;,
 4;180,174,175,181;,
 4;182,180,181,183;,
 4;184,131,130,185;,
 4;186,182,183,187;,
 4;188,186,187,189;,
 4;190,191,192,193;,
 4;194,188,189,195;,
 4;196,69,75,197;,
 4;198,199,76,71;,
 4;200,79,83,201;,
 4;202,203,204,205;,
 4;206,87,91,207;,
 4;208,209,92,88;,
 4;210,95,99,211;,
 4;212,213,100,96;,
 4;214,103,107,215;,
 4;216,217,108,104;,
 4;218,111,115,219;,
 4;220,221,116,112;,
 4;222,119,123,223;,
 4;224,225,124,120;,
 4;226,127,66,227;,
 4;228,229,70,128;,
 4;53,184,185,54;,
 4;193,62,61,190;,
 4;191,0,3,192;,
 4;13,138,137,10;,
 4;1,136,139,2;,
 4;147,12,11,144;,
 4;145,18,21,146;,
 4;155,230,231,152;,
 4;153,28,31,154;,
 4;29,160,163,30;,
 4;39,162,161,36;,
 4;171,38,37,168;,
 4;169,44,47,170;,
 4;45,176,179,46;,
 4;55,178,177,52;;
 
 MeshMaterialList {
  3;
  111;
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
  1,
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
  1,
  2,
  1,
  2,
  2,
  1,
  2,
  1,
  2,
  2,
  1,
  2,
  2,
  1,
  2,
  2,
  1,
  2,
  2,
  1,
  2,
  2,
  1,
  2,
  0,
  0,
  0,
  2,
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
   0.800000;0.589804;0.307451;1.000000;;
   5.000000;
   0.050000;0.050000;0.050000;;
   0.050000;0.036863;0.019216;;
   TextureFilename {
    "data\\TEXTURE\\Material\\Rough_Horizontal.PNG";
   }
  }
  Material {
   0.599200;0.339200;0.072000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Material\\Rough_Horizontal.PNG";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  80;
  -0.980785;-0.195090;0.000000;,
  -0.562178;-0.827014;0.002177;,
  0.195090;-0.980785;-0.000000;,
  0.831469;-0.555571;-0.000000;,
  0.980785;0.195090;0.000000;,
  0.555571;0.831469;0.000000;,
  -0.199570;0.979883;-0.001287;,
  -0.831471;0.555569;0.000000;,
  1.000000;-0.000000;-0.000001;,
  0.923880;0.382682;-0.000001;,
  0.707106;0.707107;-0.000000;,
  0.382683;0.923880;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.382683;0.923880;0.000000;,
  -0.707106;0.707107;0.000000;,
  -0.923880;0.382683;0.000000;,
  -1.000000;0.000000;0.000000;,
  -0.923879;-0.382685;0.000000;,
  -0.707107;-0.707107;0.000000;,
  -0.382685;-0.923879;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.382684;-0.923879;-0.000000;,
  0.707108;-0.707106;-0.000000;,
  0.923880;-0.382683;-0.000001;,
  -0.811267;-0.584676;0.000000;,
  -0.980785;0.195091;0.000000;,
  -0.195090;-0.980785;0.000000;,
  0.555570;-0.831470;-0.000000;,
  0.831470;0.555569;0.000000;,
  0.239330;0.970938;0.000000;,
  0.980786;-0.195088;-0.000000;,
  -0.555571;0.831469;0.000000;,
  -0.831468;-0.555572;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  -0.001721;-0.012391;-0.999922;,
  -0.001147;-0.008261;-0.999965;,
  -0.008462;-0.020301;-0.999758;,
  -0.007896;-0.007516;-0.999941;,
  -0.006742;-0.007911;-0.999946;,
  -0.006748;0.000746;-0.999977;,
  0.005813;-0.001816;0.999981;,
  0.002752;-0.008500;0.999960;,
  0.004146;-0.008649;0.999954;,
  0.000251;-0.018749;0.999824;,
  -0.001667;-0.006833;0.999975;,
  -0.002501;-0.010249;0.999944;,
  0.923880;0.382683;-0.000000;,
  1.000000;-0.000000;-0.000000;,
  0.709919;0.704283;-0.001109;,
  0.562178;0.827014;-0.002177;,
  -0.382682;0.923880;0.000000;,
  -0.195090;0.980785;0.000000;,
  -0.707106;0.707108;0.000000;,
  -0.923879;0.382684;0.000000;,
  -1.000000;-0.000000;0.000000;,
  -0.923879;-0.382684;0.000000;,
  -0.707107;-0.707107;0.000000;,
  -0.384293;-0.923211;0.000500;,
  0.000543;-0.999999;0.001157;,
  0.384794;-0.923002;0.000656;,
  0.707108;-0.707106;-0.000000;,
  0.923880;-0.382683;-0.000000;,
  -0.006760;0.018057;-0.999814;,
  0.011935;0.011553;0.999862;,
  0.850630;0.525765;-0.000000;,
  0.266558;0.963819;0.000000;,
  -0.122567;-0.992460;0.000000;,
  0.588038;-0.808834;-0.000000;,
  -0.522227;0.852807;0.000000;,
  -0.982294;0.187265;0.005529;,
  0.985031;-0.172379;-0.000000;,
  -0.797717;-0.603032;0.000000;,
  0.862398;0.506231;-0.000000;,
  -0.144964;-0.989437;0.000000;,
  0.244717;0.969595;0.000000;,
  0.594150;-0.804354;-0.000000;,
  -0.515752;0.856738;0.000000;,
  0.977793;-0.209575;-0.000000;,
  -0.982406;0.186755;-0.000962;;
  111;
  4;0,0,0,0;,
  4;9,8,8,9;,
  4;10,9,9,10;,
  4;1,1,1,1;,
  4;11,10,10,11;,
  4;12,11,11,12;,
  4;2,2,2,2;,
  4;13,12,12,13;,
  4;14,13,13,14;,
  4;3,3,3,3;,
  4;15,14,14,15;,
  4;16,15,15,16;,
  4;4,4,4,4;,
  4;17,16,16,17;,
  4;18,17,17,18;,
  4;5,5,5,5;,
  4;19,18,18,19;,
  4;20,19,19,20;,
  4;6,6,6,6;,
  4;21,20,20,21;,
  4;22,21,21,22;,
  4;7,7,7,7;,
  4;23,22,22,23;,
  4;8,23,23,8;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;33,33,35,36;,
  4;34,34,34,34;,
  4;36,35,37,38;,
  4;34,34,34,34;,
  4;38,37,39,40;,
  4;34,34,34,34;,
  4;40,39,33,33;,
  4;34,34,34,34;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;33,33,33,33;,
  4;34,41,42,34;,
  4;33,33,33,33;,
  4;41,43,44,42;,
  4;33,33,33,33;,
  4;43,45,46,44;,
  4;33,33,33,33;,
  4;45,34,34,46;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;24,24,24,24;,
  4;47,48,48,47;,
  4;32,32,32,32;,
  4;49,47,47,49;,
  4;50,49,49,50;,
  4;26,26,26,26;,
  4;51,52,52,51;,
  4;27,27,27,27;,
  4;53,51,51,53;,
  4;54,53,53,54;,
  4;30,30,30,30;,
  4;55,54,54,55;,
  4;56,55,55,56;,
  4;28,28,28,28;,
  4;57,56,56,57;,
  4;58,57,57,58;,
  4;29,29,29,29;,
  4;59,58,58,59;,
  4;60,59,59,60;,
  4;31,31,31,31;,
  4;61,60,60,61;,
  4;62,61,61,62;,
  4;25,25,25,25;,
  4;48,62,62,48;,
  4;33,33,36,33;,
  4;34,34,34,34;,
  4;63,38,40,63;,
  4;34,34,34,34;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;33,33,33,33;,
  4;64,64,43,41;,
  4;33,33,33,33;,
  4;34,34,34,45;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;65,65,65,65;,
  4;66,66,66,66;,
  4;67,67,67,67;,
  4;68,68,68,68;,
  4;69,69,69,69;,
  4;70,70,70,70;,
  4;71,71,71,71;,
  4;72,72,72,72;,
  4;73,73,73,73;,
  4;74,74,74,74;,
  4;75,75,75,75;,
  4;76,76,76,76;,
  4;77,77,77,77;,
  4;78,78,78,78;,
  4;79,79,79,79;;
 }
 MeshTextureCoords {
  232;
  0.749624;0.238136;,
  0.812122;0.238136;,
  0.812122;0.755796;,
  0.749624;0.755796;,
  0.812028;0.238136;,
  0.749530;0.238136;,
  0.749530;0.755796;,
  0.812028;0.755796;,
  0.874598;0.238136;,
  0.874598;0.755796;,
  0.874678;0.238136;,
  0.934914;0.241211;,
  0.937283;0.755796;,
  0.874678;0.755796;,
  0.937229;0.238136;,
  0.937229;0.755796;,
  0.999902;0.238136;,
  0.999902;0.755796;,
  0.999922;0.238136;,
  1.062572;0.238136;,
  1.062572;0.755796;,
  0.999922;0.755796;,
  0.062591;0.238136;,
  -0.000098;0.238136;,
  -0.000098;0.755796;,
  0.062591;0.755796;,
  0.125264;0.238136;,
  0.125265;0.755796;,
  0.125211;0.238136;,
  0.187818;0.238136;,
  0.187818;0.755796;,
  0.125211;0.755796;,
  0.187898;0.238136;,
  0.187898;0.755796;,
  0.250471;0.238136;,
  0.250471;0.755796;,
  0.250377;0.238136;,
  0.312877;0.238136;,
  0.312877;0.755796;,
  0.250377;0.755796;,
  0.312972;0.238136;,
  0.312972;0.755796;,
  0.375401;0.238136;,
  0.375401;0.755796;,
  0.375321;0.238136;,
  0.437716;0.238136;,
  0.437716;0.755796;,
  0.375321;0.755796;,
  0.437769;0.238136;,
  0.437769;0.755796;,
  0.500097;0.238136;,
  0.500097;0.755796;,
  0.500078;0.238136;,
  0.562428;0.238136;,
  0.562428;0.755796;,
  0.500410;0.758387;,
  0.562410;0.238136;,
  0.562410;0.755796;,
  0.624737;0.238136;,
  0.624737;0.755796;,
  0.624789;0.238136;,
  0.687182;0.238136;,
  0.687182;0.755796;,
  0.624789;0.755796;,
  0.687103;0.238136;,
  0.687103;0.755796;,
  -0.394492;0.497885;,
  -0.215681;0.497885;,
  -0.161237;0.771597;,
  -0.326436;0.840026;,
  -0.394492;0.497885;,
  -0.326436;0.840026;,
  -0.161236;0.771597;,
  -0.215681;0.497885;,
  -0.006191;1.003639;,
  -0.132630;1.130078;,
  -0.132629;1.130078;,
  -0.006191;1.003639;,
  0.225851;1.158684;,
  0.144031;1.321385;,
  0.157422;1.323884;,
  0.225851;1.158684;,
  0.499563;1.213129;,
  0.499563;1.391940;,
  0.499563;1.391940;,
  0.499563;1.213129;,
  0.773275;1.158684;,
  0.841702;1.323884;,
  0.841702;1.323884;,
  0.773275;1.158684;,
  1.005315;1.003639;,
  1.131754;1.130078;,
  1.131754;1.130078;,
  1.005317;1.003639;,
  1.160361;0.771597;,
  1.325562;0.840026;,
  1.325562;0.840026;,
  1.160361;0.771597;,
  1.214806;0.497885;,
  1.393617;0.497885;,
  1.393617;0.497885;,
  1.214806;0.497885;,
  1.160361;0.224173;,
  1.325562;0.155746;,
  1.325562;0.155746;,
  1.160361;0.224173;,
  1.005315;-0.007868;,
  1.131754;-0.134307;,
  1.131754;-0.134307;,
  1.005317;-0.007868;,
  0.773275;-0.162914;,
  0.841702;-0.328114;,
  0.841702;-0.328114;,
  0.773275;-0.162914;,
  0.499563;-0.217359;,
  0.499563;-0.396169;,
  0.497684;-0.399051;,
  0.499563;-0.217359;,
  0.225851;-0.162914;,
  0.157422;-0.328114;,
  0.157422;-0.328114;,
  0.225851;-0.162914;,
  -0.006191;-0.007868;,
  -0.132630;-0.134307;,
  -0.132630;-0.134307;,
  -0.006191;-0.007868;,
  -0.161237;0.224174;,
  -0.326436;0.155746;,
  -0.326436;0.155746;,
  -0.161236;0.224174;,
  0.619804;0.755796;,
  0.619804;0.238136;,
  0.062500;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.062500;1.000000;,
  0.817104;0.238136;,
  0.869830;0.238136;,
  0.869830;0.755796;,
  0.817104;0.755796;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.187500;0.000000;,
  0.187500;1.000000;,
  0.942752;0.238136;,
  0.994510;0.238136;,
  0.994510;0.755796;,
  0.942752;0.755796;,
  0.312500;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.070521;0.238136;,
  0.117210;0.238136;,
  0.117210;0.755796;,
  0.070521;0.755796;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.437500;0.000000;,
  0.437500;1.000000;,
  0.195482;0.238136;,
  0.242583;0.238136;,
  0.242583;0.755796;,
  0.195482;0.755796;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.562500;0.000000;,
  0.562500;1.000000;,
  0.320403;0.238136;,
  0.367665;0.238136;,
  0.367665;0.755796;,
  0.320403;0.755796;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.687500;0.000000;,
  0.687500;1.000000;,
  0.443172;0.238136;,
  0.494198;0.238136;,
  0.494198;0.755796;,
  0.443172;0.755796;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.812500;0.000000;,
  0.812500;1.000000;,
  0.567468;0.238136;,
  0.567467;0.755796;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  0.937500;0.000000;,
  0.937500;1.000000;,
  0.691095;0.238136;,
  0.745845;0.238136;,
  0.745845;0.755796;,
  0.691095;0.755796;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  -0.513534;0.954597;,
  -0.310079;1.259087;,
  -0.513534;0.954597;,
  -0.310079;1.259087;,
  0.109687;1.537902;,
  0.461764;1.607935;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.974123;1.499056;,
  1.242917;1.319453;,
  0.974123;1.499056;,
  1.242919;1.319453;,
  1.542746;0.871846;,
  1.606446;0.551601;,
  1.542746;0.871846;,
  1.606446;0.551601;,
  1.502087;0.025352;,
  1.319777;-0.247495;,
  1.502087;0.025352;,
  1.319777;-0.247495;,
  0.885962;-0.534612;,
  0.540839;-0.619683;,
  0.885962;-0.534612;,
  0.540839;-0.619683;,
  0.042092;-0.514704;,
  -0.260882;-0.312263;,
  0.042093;-0.514704;,
  -0.260882;-0.312263;,
  -0.538295;0.097153;,
  -0.612647;0.470943;,
  -0.538295;0.097153;,
  -0.612647;0.470943;,
  0.062572;0.755796;,
  0.062572;0.238136;;
 }
}