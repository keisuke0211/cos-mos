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
 108;
 0.01361;2.55114;-0.76207;,
 1.31265;3.37038;-0.52628;,
 0.01361;2.14921;-2.06051;,
 0.01361;2.55114;-0.76207;,
 0.01361;2.14921;0.39243;,
 1.31265;3.37038;-0.52628;,
 0.01361;2.55114;-0.76207;,
 -1.28543;3.37038;-0.52628;,
 0.01361;2.55114;-0.76207;,
 0.01361;2.14921;-2.06051;,
 -1.28543;3.37038;-0.52628;,
 0.01361;1.05114;-2.79598;,
 2.11094;1.05114;-1.73022;,
 2.17922;-0.55386;-1.95635;,
 0.01361;-0.47668;-3.16665;,
 2.26361;1.05114;0.53697;,
 2.61168;-0.55386;0.73793;,
 0.01361;1.05114;1.30185;,
 0.01361;-0.55386;1.68018;,
 -2.23639;1.05114;0.53697;,
 -2.58447;-0.55386;0.73793;,
 -2.08373;1.05114;-1.73022;,
 -2.15200;-0.55386;-1.95635;,
 0.01361;1.05114;-2.79598;,
 0.01361;-0.47668;-3.16665;,
 1.75038;-2.27051;-1.75783;,
 0.01361;-2.27051;-2.69611;,
 2.26361;-2.27051;0.63519;,
 0.01361;-2.27051;1.52032;,
 -2.23639;-2.27051;0.63519;,
 -1.72317;-2.27051;-1.75783;,
 0.01361;-2.27051;-2.69611;,
 0.94879;-3.47108;-1.03370;,
 0.02246;-3.47108;-1.73806;,
 1.32150;-3.47108;0.33672;,
 0.02246;-3.47108;1.08672;,
 -1.29428;-3.47108;0.33672;,
 -0.92158;-3.47108;-1.03370;,
 0.02246;-3.47108;-1.73806;,
 -0.04542;-4.65284;0.40047;,
 -0.04542;-4.65284;0.40047;,
 -0.04542;-4.65284;0.40047;,
 -0.04542;-4.65284;0.40047;,
 -0.04542;-4.65284;0.40047;,
 -0.04542;-4.65284;0.40047;,
 -1.71325;2.38609;0.01528;,
 1.74046;2.38609;0.01528;,
 -1.66144;2.45487;-1.12771;,
 -1.66144;2.45487;-1.12771;,
 -1.71325;2.38609;0.01528;,
 -1.28543;3.37038;-0.52628;,
 1.68866;2.45487;-1.12771;,
 1.68866;2.45487;-1.12771;,
 1.31265;3.37038;-0.52628;,
 1.74046;2.38609;0.01528;,
 0.01361;1.80074;-1.97124;,
 0.90483;1.43158;-1.97124;,
 0.97825;1.50501;-2.49330;,
 0.01361;1.90458;-2.49330;,
 1.27398;0.54037;-1.97124;,
 1.37782;0.54037;-2.49330;,
 0.90483;-0.35085;-1.97124;,
 0.97825;-0.42428;-2.49330;,
 0.01361;-0.72000;-1.97124;,
 0.01361;-0.82385;-2.49330;,
 -0.87761;-0.35085;-1.97124;,
 -0.95104;-0.42428;-2.49330;,
 -1.24676;0.54037;-1.97124;,
 -1.35061;0.54037;-2.49330;,
 -0.87761;1.43158;-1.97124;,
 -0.95104;1.50501;-2.49330;,
 0.01361;1.80074;-1.97124;,
 0.01361;1.90458;-2.49330;,
 0.90483;1.43158;-3.01536;,
 0.01361;1.80074;-3.01536;,
 1.27398;0.54037;-3.01536;,
 0.90483;-0.35085;-3.01536;,
 0.01361;-0.72000;-3.01536;,
 -0.87761;-0.35085;-3.01536;,
 -1.24676;0.54037;-3.01536;,
 -0.87761;1.43158;-3.01536;,
 0.01361;1.80074;-3.01536;,
 0.69572;1.22248;-3.45795;,
 0.01361;1.50501;-3.45795;,
 0.97825;0.54037;-3.45795;,
 0.69572;-0.14174;-3.45795;,
 0.01361;-0.42428;-3.45795;,
 -0.66850;-0.14174;-3.45795;,
 -0.95104;0.54037;-3.45795;,
 -0.66850;1.22248;-3.45795;,
 0.01361;1.50501;-3.45795;,
 0.38276;0.90952;-3.75367;,
 0.01361;1.06243;-3.75367;,
 0.53567;0.54037;-3.75367;,
 0.38276;0.17121;-3.75367;,
 0.01361;0.01830;-3.75367;,
 -0.35555;0.17121;-3.75367;,
 -0.50845;0.54037;-3.75367;,
 -0.35555;0.90952;-3.75367;,
 0.01361;1.06243;-3.75367;,
 0.01361;0.54037;-3.85752;,
 0.01361;0.54037;-3.85752;,
 0.01361;0.54037;-3.85752;,
 0.01361;0.54037;-3.85752;,
 0.01361;0.54037;-3.85752;,
 0.01361;0.54037;-3.85752;,
 0.01361;0.54037;-3.85752;,
 0.01361;0.54037;-3.85752;;
 
 80;
 3;0,1,2;,
 3;3,4,5;,
 3;6,7,4;,
 3;8,9,10;,
 4;11,12,13,14;,
 4;12,15,16,13;,
 4;15,17,18,16;,
 4;17,19,20,18;,
 4;19,21,22,20;,
 4;21,23,24,22;,
 4;14,13,25,26;,
 4;13,16,27,25;,
 4;16,18,28,27;,
 4;18,20,29,28;,
 4;20,22,30,29;,
 4;22,24,31,30;,
 4;26,25,32,33;,
 4;25,27,34,32;,
 4;27,28,35,34;,
 4;28,29,36,35;,
 4;29,30,37,36;,
 4;30,31,38,37;,
 3;33,32,39;,
 3;32,34,40;,
 3;34,35,41;,
 3;35,36,42;,
 3;36,37,43;,
 3;37,38,44;,
 3;45,4,7;,
 4;4,45,19,17;,
 3;46,5,4;,
 4;15,46,4,17;,
 3;47,10,9;,
 3;48,49,50;,
 3;51,2,1;,
 3;52,53,54;,
 4;21,47,9,23;,
 4;12,11,2,51;,
 4;21,19,49,48;,
 4;12,52,54,15;,
 4;55,56,57,58;,
 4;56,59,60,57;,
 4;59,61,62,60;,
 4;61,63,64,62;,
 4;63,65,66,64;,
 4;65,67,68,66;,
 4;67,69,70,68;,
 4;69,71,72,70;,
 4;58,57,73,74;,
 4;57,60,75,73;,
 4;60,62,76,75;,
 4;62,64,77,76;,
 4;64,66,78,77;,
 4;66,68,79,78;,
 4;68,70,80,79;,
 4;70,72,81,80;,
 4;74,73,82,83;,
 4;73,75,84,82;,
 4;75,76,85,84;,
 4;76,77,86,85;,
 4;77,78,87,86;,
 4;78,79,88,87;,
 4;79,80,89,88;,
 4;80,81,90,89;,
 4;83,82,91,92;,
 4;82,84,93,91;,
 4;84,85,94,93;,
 4;85,86,95,94;,
 4;86,87,96,95;,
 4;87,88,97,96;,
 4;88,89,98,97;,
 4;89,90,99,98;,
 3;92,91,100;,
 3;91,93,101;,
 3;93,94,102;,
 3;94,95,103;,
 3;95,96,104;,
 3;96,97,105;,
 3;97,98,106;,
 3;98,99,107;;
 
 MeshMaterialList {
  4;
  80;
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
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  3,
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.323200;0.323200;0.323200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.571200;0.571200;0.571200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.149940;0.149940;0.149940;;
  }
  Material {
   0.021961;0.345098;0.379608;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  91;
  -0.534477;0.845144;0.008190;,
  0.000000;0.734371;-0.678748;,
  -0.117674;0.613738;0.780691;,
  0.534477;0.845144;0.008190;,
  0.000001;0.370698;-0.928753;,
  0.794751;0.315642;-0.518402;,
  0.971808;0.222347;-0.078431;,
  0.000000;0.379738;0.925094;,
  -0.266000;0.366058;0.891765;,
  -0.794751;0.315642;-0.518402;,
  0.000000;-0.026451;-0.999650;,
  0.818412;-0.041946;-0.573099;,
  0.987660;-0.051737;-0.147823;,
  0.000000;0.052595;0.998616;,
  -0.342199;0.049419;0.938327;,
  -0.818412;-0.041947;-0.573099;,
  0.000968;-0.484658;-0.874703;,
  0.721477;-0.458952;-0.518493;,
  0.886259;-0.423431;-0.187751;,
  0.000879;-0.266575;0.963814;,
  -0.380425;-0.247349;0.891121;,
  -0.720821;-0.459499;-0.518920;,
  -0.000545;-0.790967;-0.611858;,
  0.566904;-0.729479;-0.382728;,
  0.710412;-0.679923;-0.181713;,
  -0.000596;-0.459704;0.888072;,
  -0.424120;-0.407150;0.808920;,
  -0.575610;-0.719130;-0.389262;,
  0.234612;-0.885667;-0.400688;,
  -0.156977;0.517642;0.841073;,
  0.156978;0.517642;0.841073;,
  -0.727208;0.515251;-0.453525;,
  0.727211;0.515249;-0.453523;,
  0.548082;0.789923;0.275005;,
  0.266001;0.366058;0.891765;,
  0.342199;0.049419;0.938327;,
  -0.971807;0.222348;-0.078431;,
  -0.987660;-0.051738;-0.147823;,
  0.381972;-0.245546;0.890957;,
  -0.886260;-0.423431;-0.187749;,
  0.423431;-0.425575;0.799745;,
  -0.724806;-0.663469;-0.185646;,
  -0.002999;-0.502041;0.864839;,
  -0.670859;-0.718792;-0.182447;,
  -0.114456;0.516614;0.848534;,
  0.114456;0.516614;0.848534;,
  -0.698386;0.570636;-0.432009;,
  -0.934136;0.355545;-0.031272;,
  0.698389;0.570635;-0.432005;,
  0.934137;0.355542;-0.031268;,
  0.000001;0.980786;0.195086;,
  0.693522;0.693520;0.195081;,
  0.980788;0.000002;0.195079;,
  0.693521;-0.693519;0.195086;,
  0.000001;-0.980784;0.195095;,
  -0.693519;-0.693519;0.195097;,
  -0.980784;0.000002;0.195097;,
  -0.693520;0.693520;0.195092;,
  0.000001;1.000000;-0.000000;,
  0.707108;0.707106;0.000000;,
  1.000000;0.000002;0.000000;,
  0.707108;-0.707106;-0.000000;,
  0.000001;-1.000000;-0.000000;,
  -0.707107;-0.707107;0.000000;,
  -1.000000;0.000002;0.000000;,
  -0.707107;0.707107;0.000000;,
  0.000001;0.923106;-0.384545;,
  0.652736;0.652734;-0.384544;,
  0.923107;0.000002;-0.384544;,
  0.652735;-0.652733;-0.384547;,
  0.000001;-0.923105;-0.384549;,
  -0.652733;-0.652733;-0.384550;,
  -0.923105;0.000002;-0.384549;,
  -0.652734;0.652734;-0.384547;,
  0.000001;0.704977;-0.709230;,
  0.498493;0.498492;-0.709232;,
  0.704975;0.000001;-0.709232;,
  0.498495;-0.498492;-0.709232;,
  0.000001;-0.704979;-0.709228;,
  -0.498496;-0.498494;-0.709229;,
  -0.704978;0.000001;-0.709229;,
  -0.498493;0.498494;-0.709231;,
  0.000001;0.380612;-0.924735;,
  0.269134;0.269134;-0.924735;,
  0.380612;0.000001;-0.924735;,
  0.269135;-0.269132;-0.924735;,
  0.000001;-0.380612;-0.924735;,
  -0.269135;-0.269131;-0.924735;,
  -0.380613;0.000001;-0.924734;,
  -0.269132;0.269131;-0.924736;,
  0.000001;0.000001;-1.000000;;
  80;
  3;0,0,1;,
  3;0,2,0;,
  3;3,3,33;,
  3;3,1,3;,
  4;4,5,11,10;,
  4;5,6,12,11;,
  4;34,7,13,35;,
  4;7,8,14,13;,
  4;36,9,15,37;,
  4;9,4,10,15;,
  4;10,11,17,16;,
  4;11,12,18,17;,
  4;35,13,19,38;,
  4;13,14,20,19;,
  4;37,15,21,39;,
  4;15,10,16,21;,
  4;16,17,23,22;,
  4;17,18,24,23;,
  4;38,19,25,40;,
  4;19,20,26,25;,
  4;39,21,27,41;,
  4;21,16,22,27;,
  3;22,23,28;,
  3;23,24,28;,
  3;40,25,42;,
  3;25,26,42;,
  3;41,27,43;,
  3;27,22,28;,
  3;29,2,44;,
  4;2,29,8,7;,
  3;30,45,2;,
  4;34,30,2,7;,
  3;31,46,1;,
  3;31,47,46;,
  3;32,1,48;,
  3;32,48,49;,
  4;9,31,1,4;,
  4;5,4,1,32;,
  4;9,36,47,31;,
  4;5,32,49,6;,
  4;50,51,59,58;,
  4;51,52,60,59;,
  4;52,53,61,60;,
  4;53,54,62,61;,
  4;54,55,63,62;,
  4;55,56,64,63;,
  4;56,57,65,64;,
  4;57,50,58,65;,
  4;58,59,67,66;,
  4;59,60,68,67;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,63,71,70;,
  4;63,64,72,71;,
  4;64,65,73,72;,
  4;65,58,66,73;,
  4;66,67,75,74;,
  4;67,68,76,75;,
  4;68,69,77,76;,
  4;69,70,78,77;,
  4;70,71,79,78;,
  4;71,72,80,79;,
  4;72,73,81,80;,
  4;73,66,74,81;,
  4;74,75,83,82;,
  4;75,76,84,83;,
  4;76,77,85,84;,
  4;77,78,86,85;,
  4;78,79,87,86;,
  4;79,80,88,87;,
  4;80,81,89,88;,
  4;81,74,82,89;,
  3;82,83,90;,
  3;83,84,90;,
  3;84,85,90;,
  3;85,86,90;,
  3;86,87,90;,
  3;87,88,90;,
  3;88,89,90;,
  3;89,82,90;;
 }
 MeshTextureCoords {
  108;
  0.083330;0.000000;,
  0.166670;0.166670;,
  0.000000;0.166670;,
  0.416670;0.000000;,
  0.500000;0.166670;,
  0.333330;0.166670;,
  0.583330;0.000000;,
  0.666670;0.166670;,
  0.916670;0.000000;,
  1.000000;0.166670;,
  0.833330;0.166670;,
  0.000000;0.333330;,
  0.166670;0.333330;,
  0.166670;0.500000;,
  0.000000;0.500000;,
  0.333330;0.333330;,
  0.333330;0.500000;,
  0.500000;0.333330;,
  0.500000;0.500000;,
  0.666670;0.333330;,
  0.666670;0.500000;,
  0.833330;0.333330;,
  0.833330;0.500000;,
  1.000000;0.333330;,
  1.000000;0.500000;,
  0.166670;0.666670;,
  0.000000;0.666670;,
  0.333330;0.666670;,
  0.500000;0.666670;,
  0.666670;0.666670;,
  0.833330;0.666670;,
  1.000000;0.666670;,
  0.166670;0.833330;,
  0.000000;0.833330;,
  0.333330;0.833330;,
  0.500000;0.833330;,
  0.666670;0.833330;,
  0.833330;0.833330;,
  1.000000;0.833330;,
  0.083330;1.000000;,
  0.250000;1.000000;,
  0.416670;1.000000;,
  0.583330;1.000000;,
  0.750000;1.000000;,
  0.916670;1.000000;,
  0.666670;0.240020;,
  0.333330;0.238000;,
  0.833330;0.232340;,
  0.782840;0.232340;,
  0.713320;0.240020;,
  0.750000;0.166670;,
  0.166670;0.232340;,
  0.217160;0.232340;,
  0.250000;0.166670;,
  0.285670;0.238000;,
  0.000000;0.375000;,
  0.125000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.375000;,
  0.250000;0.500000;,
  0.375000;0.375000;,
  0.375000;0.500000;,
  0.500000;0.375000;,
  0.500000;0.500000;,
  0.625000;0.375000;,
  0.625000;0.500000;,
  0.750000;0.375000;,
  0.750000;0.500000;,
  0.875000;0.375000;,
  0.875000;0.500000;,
  1.000000;0.375000;,
  1.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
}
