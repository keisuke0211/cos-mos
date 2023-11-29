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
 268;
 -8.00000;-0.06762;8.00000;,
 -8.00000;8.01148;8.00000;,
 -8.00000;8.01148;-8.00000;,
 -8.00000;-0.06762;-8.00000;,
 8.00000;8.01148;-8.00000;,
 8.00000;8.01148;8.00000;,
 8.00000;-0.06762;8.00000;,
 8.00000;-0.06762;-8.00000;,
 -8.00000;-7.98851;-8.00000;,
 -8.00000;-7.98851;8.00000;,
 -8.00000;-6.85939;8.00000;,
 -8.00000;-6.85939;-8.00000;,
 8.00000;-6.85939;8.00000;,
 8.00000;-6.85939;-8.00000;,
 8.00000;-7.98851;8.00000;,
 8.00000;-7.98851;-8.00000;,
 0.37710;-7.98851;-8.00000;,
 -0.40502;-7.98851;-8.00000;,
 -0.40502;-5.98053;-8.00000;,
 0.37710;-5.98053;-8.00000;,
 -0.40502;-5.98053;-8.00000;,
 -0.03605;-0.06762;-8.00000;,
 0.37710;-5.98053;-8.00000;,
 -0.03605;-7.98851;8.00000;,
 -0.40502;-7.98851;-8.00000;,
 0.37710;-7.98851;-8.00000;,
 -0.40502;-7.98851;-8.00000;,
 -1.21372;-7.98851;-8.00000;,
 -1.21372;-5.98053;-8.00000;,
 -0.03605;-7.98851;8.00000;,
 -1.21372;-7.98851;-8.00000;,
 -1.21372;-5.98053;-8.00000;,
 -1.21372;-7.98851;-8.00000;,
 -2.04221;-7.98851;-8.00000;,
 -2.04221;-5.98053;-8.00000;,
 -2.04221;-7.98851;-8.00000;,
 -8.00000;-7.98851;-8.00000;,
 -8.00000;-6.85939;-8.00000;,
 -2.04221;-5.98053;-8.00000;,
 -8.00000;-0.06762;-8.00000;,
 -0.03605;-7.98851;8.00000;,
 -2.04221;-7.98851;-8.00000;,
 -8.00000;-7.98851;-8.00000;,
 -8.00000;8.01148;-8.00000;,
 -0.03605;8.01148;-8.00000;,
 -0.03605;-6.85939;8.00000;,
 -8.00000;-6.85939;8.00000;,
 -8.00000;-7.98851;8.00000;,
 -0.03605;-7.98851;8.00000;,
 -8.00000;8.01148;-8.00000;,
 -0.03600;8.00000;8.00000;,
 -0.03605;8.01148;-8.00000;,
 -0.03605;-0.06762;8.00000;,
 -8.00000;-0.06762;8.00000;,
 -8.00000;8.01148;8.00000;,
 -0.03600;8.00000;8.00000;,
 1.18287;-7.98851;-8.00000;,
 0.37710;-7.98851;-8.00000;,
 1.18287;-5.98053;-8.00000;,
 -0.03605;-7.98851;8.00000;,
 1.18287;-7.98851;-8.00000;,
 8.00000;-6.85939;-8.00000;,
 8.00000;-7.98851;-8.00000;,
 1.92295;-7.98851;-8.00000;,
 1.92295;-5.98053;-8.00000;,
 1.92295;-7.98851;-8.00000;,
 1.18287;-7.98851;-8.00000;,
 1.18287;-5.98053;-8.00000;,
 1.92295;-5.98053;-8.00000;,
 8.00000;-0.06762;-8.00000;,
 8.00000;-7.98851;-8.00000;,
 -0.03605;-7.98851;8.00000;,
 1.92295;-7.98851;-8.00000;,
 8.00000;8.01148;-8.00000;,
 8.00000;-7.98851;8.00000;,
 8.00000;-6.85939;8.00000;,
 -0.03605;-6.85939;8.00000;,
 -0.03605;-7.98851;8.00000;,
 -0.03600;8.00000;8.00000;,
 8.00000;8.01148;-8.00000;,
 -0.03605;8.01148;-8.00000;,
 8.00000;-0.06762;8.00000;,
 -0.03605;-0.06762;8.00000;,
 8.00000;8.01148;8.00000;,
 -0.03600;8.00000;8.00000;,
 3.55451;4.55578;-9.21547;,
 3.55451;4.55578;-6.80323;,
 2.30735;4.45956;-6.80323;,
 2.30735;4.45956;-9.21547;,
 -3.46598;4.45284;-6.80323;,
 -3.46598;4.45284;-9.21547;,
 -2.21516;4.47424;-9.21547;,
 -2.21516;4.47424;-6.80323;,
 2.44115;6.80174;-9.21547;,
 3.55451;4.55578;-9.21547;,
 2.30735;4.45956;-9.21547;,
 1.69447;6.00471;-9.21547;,
 2.44115;6.80174;-9.21547;,
 2.44115;6.80174;-6.80323;,
 3.55451;4.55578;-9.21547;,
 1.69447;6.00471;-9.21547;,
 2.30735;4.45956;-9.21547;,
 1.69447;6.00471;-6.80323;,
 2.44115;6.80174;-6.80323;,
 1.69447;6.00471;-6.80323;,
 2.30735;4.45956;-6.80323;,
 3.55451;4.55578;-6.80323;,
 -2.39659;6.93241;-9.21547;,
 -1.51754;6.04768;-9.21547;,
 -2.21516;4.47424;-9.21547;,
 -3.46598;4.45284;-9.21547;,
 -2.39659;6.93241;-9.21547;,
 -3.46598;4.45284;-9.21547;,
 -2.39659;6.93241;-6.80323;,
 -1.51754;6.04768;-9.21547;,
 -1.51754;6.04768;-6.80323;,
 -2.21516;4.47424;-9.21547;,
 -2.39659;6.93241;-6.80323;,
 -3.46598;4.45284;-6.80323;,
 -2.21516;4.47424;-6.80323;,
 -1.51754;6.04768;-6.80323;,
 0.05015;7.94086;-9.21547;,
 0.05015;6.71456;-9.21547;,
 0.05015;7.94086;-9.21547;,
 0.05015;7.94086;-6.80323;,
 0.05015;6.71456;-9.21547;,
 0.05015;6.71456;-6.80323;,
 0.05015;7.94086;-6.80323;,
 0.05015;6.71456;-6.80323;,
 0.05395;4.42561;-8.07894;,
 1.63762;2.84987;-8.05129;,
 0.05395;2.19719;-8.03984;,
 0.05395;4.42561;-8.07894;,
 2.29360;4.42561;-8.07894;,
 0.05395;4.42561;-8.07894;,
 1.63762;6.00135;-8.10658;,
 0.05395;4.42561;-8.07894;,
 0.05395;6.65404;-8.11803;,
 0.05395;4.42561;-8.07894;,
 -1.52972;6.00135;-8.10658;,
 0.05395;4.42561;-8.07894;,
 -2.18570;4.42561;-8.07894;,
 0.05395;4.42561;-8.07894;,
 -1.52972;2.84987;-8.05129;,
 0.05395;4.42561;-8.07894;,
 0.05395;2.19719;-8.03984;,
 3.39675;-7.98171;-9.21547;,
 3.39675;-7.98171;-6.80323;,
 3.83192;-6.81224;-6.80323;,
 3.83192;-6.81224;-9.21547;,
 6.20635;-1.39253;-6.80323;,
 6.24135;-1.62668;-9.21547;,
 5.73172;-2.72324;-9.21547;,
 5.73844;-2.76823;-6.80323;,
 5.84844;-7.91136;-9.21547;,
 3.39675;-7.98171;-9.21547;,
 3.83192;-6.81224;-9.21547;,
 5.45475;-6.90245;-9.21547;,
 5.84844;-7.91136;-9.21547;,
 5.84844;-7.91136;-6.80323;,
 3.39675;-7.98171;-9.21547;,
 5.45475;-6.90245;-9.21547;,
 3.83192;-6.81224;-9.21547;,
 5.45475;-6.90245;-6.80323;,
 5.84844;-7.91136;-6.80323;,
 5.45475;-6.90245;-6.80323;,
 3.83192;-6.81224;-6.80323;,
 3.39675;-7.98171;-6.80323;,
 7.98235;-3.58537;-9.21547;,
 6.83282;-4.01192;-9.21547;,
 5.73172;-2.72324;-9.21547;,
 6.24135;-1.62668;-9.21547;,
 7.98235;-3.58537;-9.21547;,
 6.24135;-1.62668;-9.21547;,
 7.98235;-3.58537;-6.80323;,
 6.83282;-4.01192;-9.21547;,
 6.83282;-4.01192;-6.80323;,
 5.73172;-2.72324;-9.21547;,
 7.98235;-3.58537;-6.80323;,
 6.20635;-1.39253;-6.80323;,
 5.73844;-2.76823;-6.80323;,
 6.83282;-4.01192;-6.80323;,
 7.85384;-6.22195;-9.21547;,
 6.76882;-5.70991;-9.21547;,
 7.85384;-6.22195;-9.21547;,
 7.85384;-6.22195;-6.80323;,
 6.76882;-5.70991;-9.21547;,
 6.76882;-5.70991;-6.80323;,
 7.85384;-6.22195;-6.80323;,
 6.76882;-5.70991;-6.80323;,
 4.74202;-4.75761;-8.07894;,
 2.68712;-5.53368;-8.05129;,
 2.77034;-3.82714;-8.03984;,
 4.74202;-4.75761;-8.07894;,
 3.80764;-6.78561;-8.07894;,
 4.74202;-4.75761;-8.07894;,
 5.47549;-6.84957;-8.10658;,
 4.74202;-4.75761;-8.07894;,
 6.71369;-5.68808;-8.11803;,
 4.74202;-4.75761;-8.07894;,
 6.79691;-3.98155;-8.10658;,
 4.74202;-4.75761;-8.07894;,
 5.68312;-2.77461;-8.07894;,
 4.74202;-4.75761;-8.07894;,
 4.01526;-2.71064;-8.05129;,
 4.74202;-4.75761;-8.07894;,
 2.77034;-3.82714;-8.03984;,
 -6.21599;-1.62291;-9.21547;,
 -6.21599;-1.62291;-6.80323;,
 -5.62847;-2.72374;-6.80323;,
 -5.62847;-2.72374;-9.21547;,
 -3.14268;-8.09330;-6.80323;,
 -3.34124;-7.96436;-9.21547;,
 -3.82483;-6.85606;-9.21547;,
 -3.86297;-6.83129;-6.80323;,
 -7.78784;-3.50573;-9.21547;,
 -6.21599;-1.62291;-9.21547;,
 -5.62847;-2.72374;-9.21547;,
 -6.77134;-3.87940;-9.21547;,
 -7.78784;-3.50573;-9.21547;,
 -7.78784;-3.50573;-6.80323;,
 -6.21599;-1.62291;-9.21547;,
 -6.77134;-3.87940;-9.21547;,
 -5.62847;-2.72374;-9.21547;,
 -6.77134;-3.87940;-6.80323;,
 -7.78784;-3.50573;-6.80323;,
 -6.77134;-3.87940;-6.80323;,
 -5.62847;-2.72374;-6.80323;,
 -6.21599;-1.62291;-6.80323;,
 -5.96184;-7.97042;-9.21547;,
 -5.51960;-6.82684;-9.21547;,
 -3.82483;-6.85606;-9.21547;,
 -3.34124;-7.96436;-9.21547;,
 -5.96184;-7.97042;-9.21547;,
 -3.34124;-7.96436;-9.21547;,
 -5.96184;-7.97042;-6.80323;,
 -5.51960;-6.82684;-9.21547;,
 -5.51960;-6.82684;-6.80323;,
 -3.82483;-6.85606;-9.21547;,
 -5.96184;-7.97042;-6.80323;,
 -3.14268;-8.09330;-6.80323;,
 -3.86297;-6.83129;-6.80323;,
 -5.51960;-6.82684;-6.80323;,
 -7.85137;-6.12712;-9.21547;,
 -6.74891;-5.65378;-9.21547;,
 -7.85137;-6.12712;-9.21547;,
 -7.85137;-6.12712;-6.80323;,
 -6.74891;-5.65378;-9.21547;,
 -6.74891;-5.65378;-6.80323;,
 -7.85137;-6.12712;-6.80323;,
 -6.74891;-5.65378;-6.80323;,
 -4.69269;-4.76681;-8.07894;,
 -3.91231;-2.71354;-8.05129;,
 -2.68933;-3.90665;-8.03984;,
 -4.69269;-4.76681;-8.07894;,
 -5.59243;-2.72321;-8.07894;,
 -4.69269;-4.76681;-8.07894;,
 -6.74548;-3.92997;-8.10658;,
 -4.69269;-4.76681;-8.07894;,
 -6.69604;-5.62695;-8.11803;,
 -4.69269;-4.76681;-8.07894;,
 -5.47306;-6.82006;-8.10658;,
 -4.69269;-4.76681;-8.07894;,
 -3.83110;-6.78563;-8.07894;,
 -4.69269;-4.76681;-8.07894;,
 -2.67803;-5.57886;-8.05129;,
 -4.69269;-4.76681;-8.07894;,
 -2.68933;-3.90665;-8.03984;;
 
 142;
 3;0,1,2;,
 3;0,2,3;,
 3;4,5,6;,
 3;4,6,7;,
 3;8,9,10;,
 3;8,10,11;,
 3;10,0,3;,
 3;10,3,11;,
 3;7,6,12;,
 3;7,12,13;,
 3;12,14,15;,
 3;12,15,13;,
 3;16,17,18;,
 3;16,18,19;,
 3;20,21,22;,
 3;23,24,25;,
 3;20,26,27;,
 3;20,27,28;,
 3;21,20,28;,
 3;24,29,30;,
 3;31,32,33;,
 3;31,33,34;,
 3;35,36,37;,
 3;35,37,38;,
 3;37,39,21;,
 3;37,21,38;,
 3;21,28,38;,
 3;30,40,41;,
 3;40,9,42;,
 3;40,42,41;,
 3;39,43,44;,
 3;39,44,21;,
 3;45,46,47;,
 3;45,47,48;,
 3;49,1,50;,
 3;49,50,51;,
 3;52,53,46;,
 3;52,46,45;,
 3;54,53,52;,
 3;54,52,55;,
 3;56,57,22;,
 3;56,22,58;,
 3;22,21,58;,
 3;59,25,60;,
 3;61,62,63;,
 3;61,63,64;,
 3;65,66,67;,
 3;65,67,68;,
 3;58,21,64;,
 3;21,69,61;,
 3;21,61,64;,
 3;70,14,71;,
 3;70,71,72;,
 3;71,60,72;,
 3;44,73,69;,
 3;44,69,21;,
 3;74,75,76;,
 3;74,76,77;,
 3;78,5,79;,
 3;78,79,80;,
 3;75,81,82;,
 3;75,82,76;,
 3;82,81,83;,
 3;82,83,84;,
 4;85,86,87,88;,
 4;89,90,91,92;,
 4;93,94,95,96;,
 4;97,98,86,99;,
 4;100,101,87,102;,
 4;103,104,105,106;,
 4;107,108,109,110;,
 4;111,112,89,113;,
 4;114,115,92,116;,
 4;117,118,119,120;,
 4;121,93,96,122;,
 4;121,122,108,107;,
 4;123,111,113,124;,
 4;123,124,98,97;,
 4;125,100,102,126;,
 4;125,126,115,114;,
 4;127,117,120,128;,
 4;127,128,104,103;,
 3;129,130,131;,
 3;132,133,130;,
 3;134,135,133;,
 3;136,137,135;,
 3;138,139,137;,
 3;140,141,139;,
 3;142,143,141;,
 3;144,145,143;,
 4;146,147,148,149;,
 4;150,151,152,153;,
 4;154,155,156,157;,
 4;158,159,147,160;,
 4;161,162,148,163;,
 4;164,165,166,167;,
 4;168,169,170,171;,
 4;172,173,150,174;,
 4;175,176,153,177;,
 4;178,179,180,181;,
 4;182,154,157,183;,
 4;182,183,169,168;,
 4;184,172,174,185;,
 4;184,185,159,158;,
 4;186,161,163,187;,
 4;186,187,176,175;,
 4;188,178,181,189;,
 4;188,189,165,164;,
 3;190,191,192;,
 3;193,194,191;,
 3;195,196,194;,
 3;197,198,196;,
 3;199,200,198;,
 3;201,202,200;,
 3;203,204,202;,
 3;205,206,204;,
 4;207,208,209,210;,
 4;211,212,213,214;,
 4;215,216,217,218;,
 4;219,220,208,221;,
 4;222,223,209,224;,
 4;225,226,227,228;,
 4;229,230,231,232;,
 4;233,234,211,235;,
 4;236,237,214,238;,
 4;239,240,241,242;,
 4;243,215,218,244;,
 4;243,244,230,229;,
 4;245,233,235,246;,
 4;245,246,220,219;,
 4;247,222,224,248;,
 4;247,248,237,236;,
 4;249,239,242,250;,
 4;249,250,226,225;,
 3;251,252,253;,
 3;254,255,252;,
 3;256,257,255;,
 3;258,259,257;,
 3;260,261,259;,
 3;262,263,261;,
 3;264,265,263;,
 3;266,267,265;;
 
 MeshMaterialList {
  3;
  142;
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
  0,
  0,
  2,
  2,
  2,
  2,
  2,
  2,
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
  0;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.671373;0.658824;0.552157;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Material\\StoneTile.PNG";
   }
  }
  Material {
   0.527059;0.495686;0.291765;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Material\\StoneTile.PNG";
   }
  }
 }
 MeshNormals {
  80;
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;1.000000;,
  0.000003;1.000000;0.000359;,
  0.000000;0.000000;-1.000000;,
  0.000721;1.000000;0.000359;,
  0.001442;0.999999;0.000000;,
  0.000000;1.000000;0.000718;,
  -0.001429;0.999999;0.000000;,
  -0.000714;1.000000;0.000359;,
  0.017110;-0.999854;-0.000000;,
  0.076922;-0.997037;-0.000000;,
  0.000000;0.000000;-1.000000;,
  0.701567;0.712604;0.000000;,
  -0.717602;-0.696453;0.000000;,
  -0.701344;0.712822;0.000000;,
  0.701738;-0.712435;-0.000000;,
  0.026826;0.999640;0.000000;,
  -0.002664;-0.999996;0.000000;,
  0.000000;-0.017541;-0.999846;,
  0.000000;-0.017543;-0.999846;,
  0.000000;-0.017543;-0.999846;,
  0.000000;-0.017542;-0.999846;,
  0.000000;-0.017539;-0.999846;,
  0.000000;-0.017539;-0.999846;,
  -0.000000;-0.017539;-0.999846;,
  -0.000000;-0.017543;-0.999846;,
  0.895957;0.444141;0.000000;,
  -0.929546;-0.368705;0.000000;,
  0.000000;0.000000;1.000000;,
  -0.918241;0.396022;0.000000;,
  0.914175;-0.405319;-0.000000;,
  -0.927975;0.372239;-0.017311;,
  -0.937216;0.348749;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.356375;-0.934343;-0.000000;,
  -0.334159;0.942517;0.000000;,
  0.946784;0.321590;-0.013383;,
  -0.943306;-0.331913;-0.002687;,
  0.896182;-0.443687;-0.000000;,
  -0.906550;0.422099;0.000000;,
  -0.016281;0.007598;-0.999839;,
  -0.016224;0.007635;-0.999839;,
  -0.016287;0.007503;-0.999839;,
  -0.016285;0.007503;-0.999839;,
  -0.016283;0.007502;-0.999839;,
  -0.016283;0.007502;-0.999839;,
  -0.016335;0.007640;-0.999837;,
  -0.016332;0.007751;-0.999837;,
  -0.016220;0.007746;-0.999838;,
  0.028683;-0.999589;-0.000000;,
  0.055499;0.998459;0.000000;,
  0.000000;0.000000;1.000000;,
  0.762214;0.646846;-0.024893;,
  -0.755512;-0.655115;-0.004998;,
  0.893685;0.448361;-0.017311;,
  0.882216;0.470845;0.000000;,
  0.000000;0.000000;-1.000000;,
  -0.935923;0.352206;0.000000;,
  0.927324;-0.374260;-0.000000;,
  -0.386501;-0.922192;-0.013383;,
  0.376464;0.926427;-0.002687;,
  -0.926130;-0.377204;0.000000;,
  0.916831;0.399275;0.000000;,
  0.016479;0.007159;-0.999839;,
  0.016469;0.007092;-0.999839;,
  0.016412;0.007226;-0.999839;,
  0.016410;0.007225;-0.999839;,
  0.016408;0.007224;-0.999839;,
  0.016408;0.007224;-0.999839;,
  0.016545;0.007172;-0.999837;,
  0.016627;0.007096;-0.999837;,
  0.016549;0.007015;-0.999838;,
  -0.767654;0.640865;0.000000;,
  0.711028;-0.703163;-0.000000;,
  0.000000;0.000000;1.000000;,
  -0.020600;-0.999478;-0.024893;,
  0.009966;0.999938;-0.004998;;
  142;
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;3,3,3;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;3,3,3;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;2,6,2;,
  3;2,2,2;,
  3;4,4,4;,
  3;4,4,4;,
  3;7,8,5;,
  3;7,5,9;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;3,3,3;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;2,2,2;,
  3;2,2,2;,
  3;4,4,4;,
  3;4,4,4;,
  3;5,10,11;,
  3;5,11,9;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  3;4,4,4;,
  4;13,13,13,13;,
  4;12,12,12,12;,
  4;14,14,14,14;,
  4;15,15,29,29;,
  4;16,30,30,16;,
  4;31,31,31,31;,
  4;14,14,14,14;,
  4;17,32,32,17;,
  4;18,18,33,33;,
  4;31,31,31,31;,
  4;14,14,14,14;,
  4;14,14,14,14;,
  4;19,17,17,19;,
  4;19,19,15,15;,
  4;20,16,16,20;,
  4;20,20,18,18;,
  4;31,31,31,31;,
  4;31,31,31,31;,
  3;21,23,22;,
  3;21,24,23;,
  3;21,25,24;,
  3;21,26,25;,
  3;21,27,26;,
  3;21,24,27;,
  3;21,28,24;,
  3;21,22,28;,
  4;35,35,35,35;,
  4;34,34,34,34;,
  4;36,36,36,36;,
  4;37,37,52,52;,
  4;38,53,53,38;,
  4;54,54,54,54;,
  4;36,36,36,36;,
  4;39,55,55,39;,
  4;40,40,56,56;,
  4;54,54,54,54;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;41,39,39,41;,
  4;41,41,37,37;,
  4;42,38,38,42;,
  4;42,42,40,40;,
  4;54,54,54,54;,
  4;54,54,54,54;,
  3;43,45,44;,
  3;43,46,45;,
  3;43,47,46;,
  3;43,48,47;,
  3;43,49,48;,
  3;43,50,49;,
  3;43,51,50;,
  3;43,44,51;,
  4;58,58,58,58;,
  4;57,57,57,57;,
  4;59,59,59,59;,
  4;60,60,75,75;,
  4;61,76,76,61;,
  4;77,77,77,77;,
  4;59,59,59,59;,
  4;62,78,78,62;,
  4;63,63,79,79;,
  4;77,77,77,77;,
  4;59,59,59,59;,
  4;59,59,59,59;,
  4;64,62,62,64;,
  4;64,64,60,60;,
  4;65,61,61,65;,
  4;65,65,63,63;,
  4;77,77,77,77;,
  4;77,77,77,77;,
  3;66,68,67;,
  3;66,69,68;,
  3;66,70,69;,
  3;66,71,70;,
  3;66,72,71;,
  3;66,73,72;,
  3;66,74,73;,
  3;66,67,74;;
 }
 MeshTextureCoords {
  268;
  0.000000;0.504944;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.504944;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.504944;,
  0.000000;0.504944;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.929430;,
  1.000000;0.929430;,
  1.000000;0.929430;,
  0.000000;0.929430;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.517757;1.000000;,
  0.479282;1.000000;,
  0.479282;0.929430;,
  0.517757;0.929430;,
  0.474686;0.873783;,
  0.497747;0.504227;,
  0.523568;0.873783;,
  0.517757;1.000000;,
  0.479282;0.000000;,
  0.517757;0.000000;,
  0.474686;0.999282;,
  0.424143;0.999282;,
  0.424143;0.873783;,
  0.439499;1.000000;,
  0.439499;0.000000;,
  0.439499;0.929430;,
  0.439499;1.000000;,
  0.398743;1.000000;,
  0.398743;0.929430;,
  0.372362;0.999282;,
  0.000000;0.999282;,
  0.000000;0.928712;,
  0.372362;0.873783;,
  0.000000;0.504227;,
  0.398743;1.000000;,
  0.398743;0.000000;,
  0.000000;0.000000;,
  0.000000;-0.000718;,
  0.497747;-0.000718;,
  0.601257;0.929430;,
  1.000000;0.929430;,
  1.000000;1.000000;,
  0.601257;1.000000;,
  0.000000;1.000000;,
  0.398743;0.000000;,
  0.398743;1.000000;,
  0.601257;0.504944;,
  1.000000;0.504944;,
  1.000000;0.000000;,
  0.601257;0.000000;,
  0.573929;0.999282;,
  0.523568;0.999282;,
  0.573929;0.873783;,
  0.557396;1.000000;,
  0.557396;0.000000;,
  1.000000;0.928712;,
  1.000000;0.999282;,
  0.620184;0.999282;,
  0.620184;0.873783;,
  0.593803;1.000000;,
  0.557396;1.000000;,
  0.557396;0.929430;,
  0.593803;0.929430;,
  1.000000;0.504227;,
  1.000000;0.000000;,
  0.593803;1.000000;,
  0.593803;0.000000;,
  1.000000;-0.000718;,
  0.000000;1.000000;,
  0.000000;0.929430;,
  0.406197;0.929430;,
  0.406197;1.000000;,
  0.593803;0.000000;,
  1.000000;1.000000;,
  0.593803;1.000000;,
  0.000000;0.504944;,
  0.406197;0.504944;,
  0.000000;0.000000;,
  0.406197;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.675710;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.675710;1.000000;,
  0.675710;1.000000;,
  0.675710;0.000000;,
  1.000000;1.000000;,
  0.675710;0.000000;,
  1.000000;0.000000;,
  0.675710;1.000000;,
  0.324290;0.000000;,
  0.324290;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.322270;0.000000;,
  0.322270;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.322270;1.000000;,
  0.000000;1.000000;,
  0.322270;0.000000;,
  0.322270;0.000000;,
  0.322270;1.000000;,
  0.000000;0.000000;,
  0.677730;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.677730;1.000000;,
  0.499590;0.000000;,
  0.499590;1.000000;,
  0.499590;1.000000;,
  0.499590;0.000000;,
  0.499590;0.000000;,
  0.499590;1.000000;,
  0.500410;0.000000;,
  0.500410;1.000000;,
  0.062500;0.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.187500;0.000000;,
  0.250000;0.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.625000;0.000000;,
  0.687500;0.000000;,
  0.750000;0.000000;,
  0.812500;0.000000;,
  0.875000;0.000000;,
  0.937500;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.675710;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.675710;1.000000;,
  0.675710;1.000000;,
  0.675710;0.000000;,
  1.000000;1.000000;,
  0.675710;0.000000;,
  1.000000;0.000000;,
  0.675710;1.000000;,
  0.324290;0.000000;,
  0.324290;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.322270;0.000000;,
  0.322270;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.322270;1.000000;,
  0.000000;1.000000;,
  0.322270;0.000000;,
  0.322270;0.000000;,
  0.322270;1.000000;,
  0.000000;0.000000;,
  0.677730;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.677730;1.000000;,
  0.499590;0.000000;,
  0.499590;1.000000;,
  0.499590;1.000000;,
  0.499590;0.000000;,
  0.499590;0.000000;,
  0.499590;1.000000;,
  0.500410;0.000000;,
  0.500410;1.000000;,
  0.062500;0.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.187500;0.000000;,
  0.250000;0.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.625000;0.000000;,
  0.687500;0.000000;,
  0.750000;0.000000;,
  0.812500;0.000000;,
  0.875000;0.000000;,
  0.937500;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.675710;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.675710;1.000000;,
  0.675710;1.000000;,
  0.675710;0.000000;,
  1.000000;1.000000;,
  0.675710;0.000000;,
  1.000000;0.000000;,
  0.675710;1.000000;,
  0.324290;0.000000;,
  0.324290;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.322270;0.000000;,
  0.322270;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.322270;1.000000;,
  0.000000;1.000000;,
  0.322270;0.000000;,
  0.322270;0.000000;,
  0.322270;1.000000;,
  0.000000;0.000000;,
  0.677730;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.677730;1.000000;,
  0.499590;0.000000;,
  0.499590;1.000000;,
  0.499590;1.000000;,
  0.499590;0.000000;,
  0.499590;0.000000;,
  0.499590;1.000000;,
  0.500410;0.000000;,
  0.500410;1.000000;,
  0.062500;0.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.187500;0.000000;,
  0.250000;0.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.625000;0.000000;,
  0.687500;0.000000;,
  0.750000;0.000000;,
  0.812500;0.000000;,
  0.875000;0.000000;,
  0.937500;0.000000;,
  1.000000;0.000000;;
 }
}
