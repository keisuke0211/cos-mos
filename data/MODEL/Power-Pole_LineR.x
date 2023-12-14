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
 292;
 0.00000;67.21216;0.00000;,
 3.24007;67.21216;-7.86134;,
 0.00000;67.21216;-8.50905;,
 5.98686;67.21216;-6.01680;,
 7.82222;67.21216;-3.25627;,
 8.46671;67.21216;0.00000;,
 7.82222;67.21216;3.25627;,
 5.98686;67.21216;6.01680;,
 3.24007;67.21216;7.86134;,
 0.00000;67.21216;8.50905;,
 -3.24007;67.21216;7.86134;,
 -5.98686;67.21216;6.01681;,
 -7.82222;67.21216;3.25627;,
 -8.46671;67.21216;0.00000;,
 -7.82222;67.21216;-3.25627;,
 -5.98686;67.21216;-6.01681;,
 -3.24007;67.21216;-7.86134;,
 0.00000;0.01010;0.00000;,
 0.00000;0.01010;-8.50905;,
 3.24007;0.01010;-7.86134;,
 5.98686;0.01010;-6.01680;,
 7.82222;0.01010;-3.25627;,
 8.46671;0.01010;0.00000;,
 7.82222;0.01010;3.25627;,
 5.98686;0.01010;6.01680;,
 3.24007;0.01010;7.86134;,
 0.00000;0.01010;8.50905;,
 -3.24007;0.01010;7.86134;,
 -5.98686;0.01010;6.01681;,
 -7.82222;0.01010;3.25627;,
 -8.46671;0.01010;0.00000;,
 -7.82222;0.01010;-3.25627;,
 -5.98686;0.01010;-6.01681;,
 -3.24007;0.01010;-7.86134;,
 -3.24007;41.40085;-7.86134;,
 -3.24007;67.21216;-7.86134;,
 0.00000;67.21216;-8.50905;,
 0.00000;41.40085;-8.50905;,
 -3.24007;41.40085;-7.86134;,
 -5.98686;41.40085;-6.01681;,
 -5.98686;67.21216;-6.01681;,
 -3.24007;67.21216;-7.86134;,
 0.00000;41.40085;-8.50905;,
 0.00000;67.21216;-8.50905;,
 3.24007;67.21216;-7.86134;,
 3.24007;41.40085;-7.86134;,
 -7.82222;41.40085;-3.25627;,
 -7.82222;67.21216;-3.25627;,
 5.98686;67.21216;-6.01680;,
 5.98686;41.40085;-6.01680;,
 -8.46671;41.40085;0.00000;,
 -8.46671;67.21216;0.00000;,
 5.98686;41.40085;-6.01680;,
 5.98686;67.21216;-6.01680;,
 7.82222;67.21216;-3.25627;,
 7.82222;41.40085;-3.25627;,
 -7.82222;41.40085;3.25627;,
 -7.82222;67.21216;3.25627;,
 8.46671;67.21216;0.00000;,
 8.46671;41.40085;0.00000;,
 -5.98686;41.40085;6.01681;,
 -5.98686;67.21216;6.01681;,
 7.82222;67.21216;3.25627;,
 7.82222;41.40085;3.25627;,
 -5.98686;41.40085;6.01681;,
 -3.24007;41.40085;7.86134;,
 -3.24007;67.21216;7.86134;,
 -5.98686;67.21216;6.01681;,
 5.98686;67.21216;6.01680;,
 5.98686;41.40085;6.01680;,
 0.00000;41.40085;8.50905;,
 0.00000;67.21216;8.50905;,
 3.24007;67.21216;7.86134;,
 3.24007;41.40085;7.86134;,
 3.24007;41.40085;7.86134;,
 3.24007;67.21216;7.86134;,
 0.00000;18.65781;-8.50905;,
 3.24007;18.65781;-7.86134;,
 0.00000;18.65781;-8.50905;,
 3.24007;18.65781;-7.86134;,
 3.24007;0.01010;-7.86134;,
 0.00000;0.01010;-8.50905;,
 -3.24007;0.01010;-7.86134;,
 -3.24007;18.65781;-7.86134;,
 0.00000;18.65781;-8.50905;,
 -3.24007;18.65781;-7.86134;,
 5.98686;18.65781;-6.01680;,
 5.98686;18.65781;-6.01680;,
 5.98686;0.01010;-6.01680;,
 -5.98686;0.01010;-6.01681;,
 -5.98686;18.65781;-6.01681;,
 -5.98686;18.65781;-6.01681;,
 -5.98686;41.40085;-6.01681;,
 7.82222;41.40085;-3.25627;,
 7.82222;18.65781;-3.25627;,
 5.98686;18.65781;-6.01680;,
 7.82222;18.65781;-3.25627;,
 7.82222;0.01010;-3.25627;,
 5.98686;0.01010;-6.01680;,
 -7.82222;0.01010;-3.25627;,
 -7.82222;18.65781;-3.25627;,
 -7.82222;18.65781;-3.25627;,
 -7.82222;41.40085;-3.25627;,
 8.46671;41.40085;0.00000;,
 8.46671;18.65781;0.00000;,
 8.46671;18.65781;0.00000;,
 8.46671;0.01010;0.00000;,
 -7.82222;18.65781;-3.25627;,
 -7.82222;0.01010;-3.25627;,
 -8.46671;0.01010;0.00000;,
 -8.46671;18.65781;0.00000;,
 -8.46671;18.65781;0.00000;,
 -8.46671;41.40085;0.00000;,
 7.82222;41.40085;3.25627;,
 7.82222;18.65781;3.25627;,
 7.82222;18.65781;3.25627;,
 7.82222;0.01010;3.25627;,
 -7.82222;0.01010;3.25627;,
 -7.82222;18.65781;3.25627;,
 -7.82222;18.65781;3.25627;,
 -7.82222;41.40085;3.25627;,
 5.98686;41.40085;6.01680;,
 5.98686;18.65781;6.01680;,
 5.98686;18.65781;6.01680;,
 5.98686;0.01010;6.01680;,
 -7.82222;18.65781;3.25627;,
 -7.82222;0.01010;3.25627;,
 -5.98686;0.01010;6.01681;,
 -5.98686;18.65781;6.01681;,
 -5.98686;18.65781;6.01681;,
 3.24007;18.65781;7.86134;,
 5.98686;18.65781;6.01680;,
 3.24007;18.65781;7.86134;,
 3.24007;0.01010;7.86134;,
 5.98686;0.01010;6.01680;,
 -3.24007;0.01010;7.86134;,
 -3.24007;18.65781;7.86134;,
 -3.24007;18.65781;7.86134;,
 0.00000;18.65781;8.50905;,
 0.00000;18.65781;8.50905;,
 0.00000;0.01010;8.50905;,
 -5.22603;59.76088;3.82348;,
 -5.24449;59.16958;2.60621;,
 -5.22603;59.76088;2.50591;,
 -5.22603;59.76088;3.82348;,
 -5.26016;58.66832;2.89182;,
 -5.22603;59.76088;3.82348;,
 -5.27063;58.33336;3.31927;,
 -5.22603;59.76088;3.82348;,
 -5.27431;58.21575;3.82348;,
 -5.22603;59.76088;3.82348;,
 -5.27063;58.33336;4.32770;,
 -5.22603;59.76088;3.82348;,
 -5.26016;58.66832;4.75515;,
 -5.22603;59.76088;3.82348;,
 -5.24450;59.16958;5.04076;,
 -5.22603;59.76088;3.82348;,
 -5.22603;59.76088;5.14105;,
 -5.22603;59.76088;3.82348;,
 -5.20754;60.35222;5.04076;,
 -5.22603;59.76088;3.82348;,
 -5.19190;60.85345;4.75515;,
 -5.22603;59.76088;3.82348;,
 -5.18144;61.18843;4.32770;,
 -5.22603;59.76088;3.82348;,
 -5.17775;61.30605;3.82348;,
 -5.22603;59.76088;3.82348;,
 -5.18144;61.18843;3.31927;,
 -5.22603;59.76088;3.82348;,
 -5.19190;60.85345;2.89182;,
 -5.22603;59.76088;3.82348;,
 -5.20754;60.35222;2.60621;,
 -5.22603;59.76088;3.82348;,
 -5.22603;59.76088;2.50591;,
 -38.32371;59.77421;3.82348;,
 -38.32371;59.77421;2.50591;,
 -38.34219;59.18290;2.60621;,
 -38.32371;59.77421;3.82348;,
 -38.35784;58.68161;2.89182;,
 -38.32371;59.77421;3.82348;,
 -38.36832;58.34665;3.31927;,
 -38.32371;59.77421;3.82348;,
 -38.37199;58.22905;3.82348;,
 -38.32371;59.77421;3.82348;,
 -38.36832;58.34665;4.32770;,
 -38.32371;59.77421;3.82348;,
 -38.35784;58.68161;4.75515;,
 -38.32371;59.77421;3.82348;,
 -38.34219;59.18290;5.04076;,
 -38.32371;59.77421;3.82348;,
 -38.32371;59.77421;5.14105;,
 -38.32371;59.77421;3.82348;,
 -38.30524;60.36551;5.04076;,
 -38.32371;59.77421;3.82348;,
 -38.28958;60.86678;4.75515;,
 -38.32371;59.77421;3.82348;,
 -38.27911;61.20175;4.32770;,
 -38.32371;59.77421;3.82348;,
 -38.27545;61.31935;3.82348;,
 -38.32371;59.77421;3.82348;,
 -38.27911;61.20175;3.31927;,
 -38.32371;59.77421;3.82348;,
 -38.28958;60.86678;2.89182;,
 -38.32371;59.77421;3.82348;,
 -38.30524;60.36551;2.60621;,
 -38.32371;59.77421;3.82348;,
 -38.32371;59.77421;2.50591;,
 -33.26046;60.47123;2.89182;,
 -33.45184;59.98619;2.60621;,
 -33.13259;60.79527;3.31927;,
 -33.67759;59.41412;2.50591;,
 -33.08768;60.90908;3.82348;,
 -33.67759;59.41412;2.50591;,
 -33.90334;58.84202;2.60621;,
 -33.13259;60.79527;4.32770;,
 -34.09471;58.35700;2.89182;,
 -33.26046;60.47123;4.75515;,
 -34.22260;58.03296;3.31927;,
 -33.45184;59.98619;5.04076;,
 -34.26750;57.91914;3.82348;,
 -33.67759;59.41412;5.14105;,
 -34.22260;58.03296;4.32770;,
 -33.90334;58.84201;5.04076;,
 -34.09471;58.35700;4.75515;,
 -8.50369;60.77940;2.89182;,
 -8.51935;60.27813;2.60621;,
 -8.49322;61.11439;3.31927;,
 -8.53782;59.68683;2.50591;,
 -8.48954;61.23199;3.82348;,
 -8.53782;59.68683;2.50591;,
 -8.55631;59.09554;2.60621;,
 -8.49322;61.11439;4.32770;,
 -8.57195;58.59426;2.89182;,
 -8.50369;60.77940;4.75515;,
 -8.58243;58.25931;3.31927;,
 -8.51935;60.27813;5.04076;,
 -8.58609;58.14169;3.82348;,
 -8.53782;59.68683;5.14105;,
 -8.58243;58.25931;4.32770;,
 -8.55631;59.09554;5.04076;,
 -8.57195;58.59426;4.75515;,
 -27.22671;58.93776;2.89182;,
 -21.05267;58.28286;2.89182;,
 -21.06832;57.78159;2.60621;,
 -27.52993;58.47857;2.60621;,
 -27.02410;59.24453;3.31927;,
 -21.04219;58.61782;3.31927;,
 -21.08680;57.19028;2.50591;,
 -27.88762;57.93699;2.50591;,
 -26.95296;59.35226;3.82348;,
 -21.03852;58.73544;3.82348;,
 -27.88762;57.93699;2.50591;,
 -21.08680;57.19028;2.50591;,
 -21.10526;56.59898;2.60621;,
 -28.24531;57.39542;2.60621;,
 -27.02410;59.24453;4.32770;,
 -21.04219;58.61782;4.32770;,
 -21.12093;56.09769;2.89182;,
 -28.54854;56.93625;2.89182;,
 -27.22671;58.93776;4.75515;,
 -21.05267;58.28286;4.75515;,
 -21.13140;55.76274;3.31927;,
 -28.75116;56.62947;3.31927;,
 -27.52993;58.47857;5.04076;,
 -21.06832;57.78159;5.04076;,
 -21.13507;55.64514;3.82348;,
 -28.82231;56.52175;3.82348;,
 -27.88762;57.93699;5.14105;,
 -21.08680;57.19028;5.14105;,
 -21.13140;55.76274;4.32770;,
 -28.75116;56.62947;4.32770;,
 -28.24531;57.39542;5.04076;,
 -21.10527;56.59898;5.04076;,
 -21.12093;56.09769;4.75515;,
 -28.54854;56.93625;4.75515;,
 -15.14244;59.27717;2.89182;,
 -14.78448;58.83577;2.60621;,
 -15.38160;59.57210;3.31927;,
 -14.36227;58.31505;2.50591;,
 -15.46557;59.67569;3.82348;,
 -14.36227;58.31505;2.50591;,
 -13.94005;57.79438;2.60621;,
 -15.38160;59.57210;4.32770;,
 -13.58212;57.35295;2.89182;,
 -15.14244;59.27717;4.75515;,
 -13.34296;57.05801;3.31927;,
 -14.78448;58.83577;5.04076;,
 -13.25897;56.95445;3.82348;,
 -14.36227;58.31505;5.14105;,
 -13.34296;57.05801;4.32770;,
 -13.94005;57.79438;5.04076;,
 -13.58212;57.35295;4.75515;;
 
 208;
 3;0,1,2;,
 3;0,3,1;,
 3;0,4,3;,
 3;0,5,4;,
 3;0,6,5;,
 3;0,7,6;,
 3;0,8,7;,
 3;0,9,8;,
 3;0,10,9;,
 3;0,11,10;,
 3;0,12,11;,
 3;0,13,12;,
 3;0,14,13;,
 3;0,15,14;,
 3;0,16,15;,
 3;0,2,16;,
 3;17,18,19;,
 3;17,19,20;,
 3;17,20,21;,
 3;17,21,22;,
 3;17,22,23;,
 3;17,23,24;,
 3;17,24,25;,
 3;17,25,26;,
 3;17,26,27;,
 3;17,27,28;,
 3;17,28,29;,
 3;17,29,30;,
 3;17,30,31;,
 3;17,31,32;,
 3;17,32,33;,
 3;17,33,18;,
 4;34,35,36,37;,
 4;38,39,40,41;,
 4;42,43,44,45;,
 4;39,46,47,40;,
 4;45,44,48,49;,
 4;46,50,51,47;,
 4;52,53,54,55;,
 4;50,56,57,51;,
 4;55,54,58,59;,
 4;56,60,61,57;,
 4;59,58,62,63;,
 4;64,65,66,67;,
 4;63,62,68,69;,
 4;65,70,71,66;,
 4;69,68,72,73;,
 4;70,74,75,71;,
 4;76,42,45,77;,
 4;78,79,80,81;,
 4;78,81,82,83;,
 4;84,85,34,37;,
 4;77,45,49,86;,
 4;79,87,88,80;,
 4;83,82,89,90;,
 4;85,91,92,34;,
 4;86,49,93,94;,
 4;95,96,97,98;,
 4;90,89,99,100;,
 4;91,101,102,92;,
 4;94,93,103,104;,
 4;96,105,106,97;,
 4;107,108,109,110;,
 4;101,111,112,102;,
 4;104,103,113,114;,
 4;105,115,116,106;,
 4;110,109,117,118;,
 4;111,119,120,112;,
 4;114,113,121,122;,
 4;115,123,124,116;,
 4;125,126,127,128;,
 4;119,129,64,120;,
 4;122,121,74,130;,
 4;131,132,133,134;,
 4;128,127,135,136;,
 4;129,137,65,64;,
 4;130,74,70,138;,
 4;132,139,140,133;,
 4;136,135,140,139;,
 4;137,138,70,65;,
 3;141,142,143;,
 3;144,145,142;,
 3;146,147,145;,
 3;148,149,147;,
 3;150,151,149;,
 3;152,153,151;,
 3;154,155,153;,
 3;156,157,155;,
 3;158,159,157;,
 3;160,161,159;,
 3;162,163,161;,
 3;164,165,163;,
 3;166,167,165;,
 3;168,169,167;,
 3;170,171,169;,
 3;172,173,171;,
 3;174,175,176;,
 3;177,176,178;,
 3;179,178,180;,
 3;181,180,182;,
 3;183,182,184;,
 3;185,184,186;,
 3;187,186,188;,
 3;189,188,190;,
 3;191,190,192;,
 3;193,192,194;,
 3;195,194,196;,
 3;197,196,198;,
 3;199,198,200;,
 3;201,200,202;,
 3;203,202,204;,
 3;205,204,206;,
 4;207,208,204,202;,
 4;207,202,200,209;,
 4;208,210,206,204;,
 4;209,200,198,211;,
 4;212,213,176,175;,
 4;211,198,196,214;,
 4;213,215,178,176;,
 4;214,196,194,216;,
 4;215,217,180,178;,
 4;216,194,192,218;,
 4;217,219,182,180;,
 4;218,192,190,220;,
 4;219,221,184,182;,
 4;220,190,188,222;,
 4;221,223,186,184;,
 4;222,188,186,223;,
 4;224,169,171,225;,
 4;224,226,167,169;,
 4;225,171,173,227;,
 4;226,228,165,167;,
 4;229,143,142,230;,
 4;228,231,163,165;,
 4;230,142,145,232;,
 4;231,233,161,163;,
 4;232,145,147,234;,
 4;233,235,159,161;,
 4;234,147,149,236;,
 4;235,237,157,159;,
 4;236,149,151,238;,
 4;237,239,155,157;,
 4;238,151,153,240;,
 4;239,240,153,155;,
 4;241,242,243,244;,
 4;241,244,208,207;,
 4;241,207,209,245;,
 4;241,245,246,242;,
 4;244,243,247,248;,
 4;244,248,210,208;,
 4;245,209,211,249;,
 4;245,249,250,246;,
 4;251,252,253,254;,
 4;251,254,213,212;,
 4;249,211,214,255;,
 4;249,255,256,250;,
 4;254,253,257,258;,
 4;254,258,215,213;,
 4;255,214,216,259;,
 4;255,259,260,256;,
 4;258,257,261,262;,
 4;258,262,217,215;,
 4;259,216,218,263;,
 4;259,263,264,260;,
 4;262,261,265,266;,
 4;262,266,219,217;,
 4;263,218,220,267;,
 4;263,267,268,264;,
 4;266,265,269,270;,
 4;266,270,221,219;,
 4;267,220,222,271;,
 4;267,271,272,268;,
 4;270,269,273,274;,
 4;270,274,223,221;,
 4;271,222,223,274;,
 4;271,274,273,272;,
 4;275,224,225,276;,
 4;275,276,243,242;,
 4;275,242,246,277;,
 4;275,277,226,224;,
 4;276,225,227,278;,
 4;276,278,247,243;,
 4;277,246,250,279;,
 4;277,279,228,226;,
 4;280,229,230,281;,
 4;280,281,253,252;,
 4;279,250,256,282;,
 4;279,282,231,228;,
 4;281,230,232,283;,
 4;281,283,257,253;,
 4;282,256,260,284;,
 4;282,284,233,231;,
 4;283,232,234,285;,
 4;283,285,261,257;,
 4;284,260,264,286;,
 4;284,286,235,233;,
 4;285,234,236,287;,
 4;285,287,265,261;,
 4;286,264,268,288;,
 4;286,288,237,235;,
 4;287,236,238,289;,
 4;287,289,269,265;,
 4;288,268,272,290;,
 4;288,290,239,237;,
 4;289,238,240,291;,
 4;289,291,273,269;,
 4;290,272,273,291;,
 4;290,291,240,239;;
 
 MeshMaterialList {
  5;
  208;
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
  2,
  2,
  2,
  2,
  0,
  2,
  2,
  1,
  1,
  2,
  2,
  0,
  0,
  2,
  2,
  1,
  1,
  2,
  2,
  0,
  0,
  2,
  2,
  1,
  1,
  2,
  2,
  0,
  0,
  2,
  2,
  1,
  1,
  2,
  2,
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
   0.429600;0.674400;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.652800;0.800000;0.724800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\power_pole.jpg";
   }
  }
  Material {
   0.351200;0.800000;0.677600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Annual_ring.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  179;
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  -0.384191;0.000000;-0.923254;,
  0.000000;0.000000;-1.000000;,
  -0.708734;0.000000;-0.705475;,
  0.384192;0.000000;-0.923253;,
  -0.924501;0.000000;-0.381180;,
  0.708735;0.000000;-0.705475;,
  -1.000000;0.000000;0.000000;,
  0.924500;0.000000;-0.381181;,
  -0.924501;0.000000;0.381180;,
  1.000000;0.000000;0.000000;,
  -0.708734;0.000000;0.705475;,
  0.924500;0.000000;0.381181;,
  -0.384191;0.000000;0.923254;,
  0.708735;0.000000;0.705475;,
  0.000000;0.000000;1.000000;,
  0.384192;0.000000;0.923253;,
  0.000000;0.000000;-1.000000;,
  0.384192;0.000000;-0.923253;,
  -0.384191;0.000000;-0.923254;,
  0.708735;0.000000;-0.705475;,
  -0.708734;0.000000;-0.705475;,
  0.924500;0.000000;0.381181;,
  -0.924501;0.000000;0.381180;,
  0.708735;0.000000;0.705475;,
  0.000000;0.000000;1.000000;,
  0.384192;0.000000;-0.923253;,
  -0.924501;0.000000;0.381180;,
  0.384192;0.000000;0.923253;,
  0.708735;0.000000;-0.705475;,
  0.924500;0.000000;-0.381181;,
  -0.924501;0.000000;-0.381180;,
  0.999512;-0.031226;0.000001;,
  0.999512;-0.031226;-0.000000;,
  0.999513;-0.031218;0.000009;,
  0.999512;-0.031234;0.000015;,
  0.999512;-0.031230;0.000011;,
  0.999512;-0.031230;0.000000;,
  0.999512;-0.031230;-0.000011;,
  0.999512;-0.031226;-0.000006;,
  0.999513;-0.031221;-0.000001;,
  0.999512;-0.031237;-0.000000;,
  0.999512;-0.031228;-0.000012;,
  0.999513;-0.031212;-0.000015;,
  0.999512;-0.031226;0.000012;,
  0.999512;-0.031232;-0.000000;,
  0.999512;-0.031226;-0.000012;,
  0.999513;-0.031212;0.000015;,
  0.999512;-0.031228;0.000012;,
  -0.999512;0.031225;0.000000;,
  -0.999512;0.031230;0.000005;,
  -0.999512;0.031233;-0.000005;,
  -0.999512;0.031238;0.000002;,
  -0.999512;0.031230;0.000000;,
  -0.999512;0.031238;-0.000002;,
  -0.999512;0.031229;0.000001;,
  -0.999512;0.031232;-0.000009;,
  -0.999512;0.031230;0.000000;,
  -0.999512;0.031222;-0.000006;,
  -0.999512;0.031227;-0.000004;,
  -0.999513;0.031220;0.000007;,
  -0.999513;0.031219;-0.000000;,
  -0.999513;0.031220;-0.000007;,
  -0.999512;0.031227;0.000004;,
  -0.999513;0.031222;0.000006;,
  -0.999512;0.031228;-0.000001;,
  -0.010062;0.337499;-0.941272;,
  -0.000093;0.000007;-1.000000;,
  -0.020058;0.654780;-0.755553;,
  0.009165;-0.337364;-0.941330;,
  -0.028194;0.902096;-0.430612;,
  0.017309;-0.654532;-0.755836;,
  -0.031465;0.999505;-0.000002;,
  0.023364;-0.902062;-0.430973;,
  -0.028194;0.902097;0.430611;,
  0.025694;-0.999670;-0.000001;,
  -0.020058;0.654780;0.755553;,
  0.023364;-0.902063;0.430972;,
  -0.010064;0.337498;0.941272;,
  0.017307;-0.654522;0.755845;,
  -0.000096;0.000021;1.000000;,
  0.009162;-0.337338;0.941339;,
  0.104850;0.638128;-0.762757;,
  0.053496;0.326589;-0.943651;,
  0.146208;0.886829;-0.438357;,
  0.000009;0.000006;-1.000000;,
  0.162907;0.986641;-0.000002;,
  -0.053414;-0.326557;-0.943667;,
  0.146208;0.886830;0.438356;,
  -0.104616;-0.638073;-0.762836;,
  0.104850;0.638128;0.762757;,
  -0.145836;-0.886835;-0.438469;,
  0.053496;0.326584;0.943653;,
  -0.162486;-0.986711;-0.000001;,
  0.000010;0.000011;1.000000;,
  -0.145836;-0.886835;0.438469;,
  -0.053413;-0.326549;0.943670;,
  -0.104617;-0.638075;0.762834;,
  -0.079913;0.648192;-0.757272;,
  -0.041654;0.333469;-0.941840;,
  -0.109311;0.895005;-0.432455;,
  0.000223;0.000065;-1.000000;,
  -0.120840;0.992672;-0.000002;,
  0.043777;-0.332849;-0.941963;,
  -0.109311;0.895006;0.432453;,
  0.086456;-0.646648;-0.757873;,
  -0.079913;0.648192;0.757272;,
  0.120919;-0.893091;-0.433322;,
  -0.041653;0.333462;0.941843;,
  0.134778;-0.990876;-0.000002;,
  0.000223;0.000065;1.000000;,
  0.120919;-0.893092;0.433320;,
  0.043776;-0.332842;0.941966;,
  0.086456;-0.646648;0.757873;,
  0.113853;0.638887;-0.760829;,
  0.058505;0.327543;-0.943023;,
  0.157777;0.885883;-0.436255;,
  -0.000095;0.000033;-1.000000;,
  0.175358;0.984505;-0.000002;,
  -0.059407;-0.327253;-0.943068;,
  0.157777;0.885884;0.436253;,
  -0.116617;-0.638134;-0.761042;,
  0.113853;0.638887;0.760829;,
  -0.162647;-0.884862;-0.436537;,
  0.058504;0.327537;0.943025;,
  -0.181187;-0.983449;-0.000002;,
  -0.000095;0.000033;1.000000;,
  -0.162648;-0.884863;0.436535;,
  -0.059406;-0.327248;0.943069;,
  -0.116617;-0.638135;0.761041;,
  -0.127129;0.645090;-0.753457;,
  -0.066092;0.333142;-0.940557;,
  -0.173974;0.886674;-0.428417;,
  0.000207;0.000051;-1.000000;,
  -0.192270;0.981342;-0.000002;,
  0.068036;-0.332567;-0.940622;,
  -0.173974;0.886675;0.428416;,
  0.133096;-0.643486;-0.753798;,
  -0.127129;0.645090;0.753457;,
  0.184520;-0.884305;-0.428901;,
  -0.066092;0.333141;0.940558;,
  0.204907;-0.978782;-0.000001;,
  0.000204;0.000065;1.000000;,
  0.184520;-0.884306;0.428901;,
  0.068031;-0.332541;0.940632;,
  0.133095;-0.643476;0.753807;,
  0.026304;0.336117;-0.941453;,
  0.051316;0.652397;-0.756138;,
  0.070999;0.899450;-0.431217;,
  0.000057;-0.000009;-1.000000;,
  0.078787;0.996891;-0.000002;,
  -0.025768;-0.336212;-0.941434;,
  0.070999;0.899451;0.431215;,
  -0.049680;-0.652633;-0.756044;,
  0.051316;0.652397;0.756138;,
  -0.068135;-0.899733;-0.431089;,
  0.026304;0.336110;0.941456;,
  -0.075371;-0.997156;0.000001;,
  0.000058;0.000002;1.000000;,
  -0.068135;-0.899733;0.431090;,
  -0.025767;-0.336196;0.941439;,
  -0.049681;-0.652635;0.756042;,
  -0.014613;0.653515;-0.756773;,
  -0.007525;0.336466;-0.941666;,
  -0.020160;0.901696;-0.431900;,
  -0.000001;-0.000003;-1.000000;,
  -0.022353;0.999750;-0.000002;,
  0.007523;-0.336462;-0.941667;,
  -0.020160;0.901697;0.431898;,
  0.014613;-0.653524;-0.756765;,
  -0.014613;0.653515;0.756773;,
  0.020163;-0.901701;-0.431890;,
  -0.007525;0.336458;0.941668;,
  0.022356;-0.999750;-0.000002;,
  -0.000001;-0.000003;1.000000;,
  0.020163;-0.901702;0.431888;,
  0.007523;-0.336454;0.941670;,
  0.014613;-0.653524;0.756765;;
  208;
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;2,2,3,3;,
  4;2,4,4,2;,
  4;3,3,27,5;,
  4;4,6,6,4;,
  4;5,27,7,7;,
  4;6,8,8,6;,
  4;7,7,9,9;,
  4;8,10,28,8;,
  4;9,9,11,11;,
  4;10,12,12,28;,
  4;11,11,13,13;,
  4;12,14,14,12;,
  4;13,13,15,15;,
  4;14,16,16,14;,
  4;15,15,29,17;,
  4;16,17,29,16;,
  4;18,3,5,19;,
  4;18,19,5,3;,
  4;18,3,2,20;,
  4;18,20,2,3;,
  4;19,5,7,21;,
  4;19,21,30,5;,
  4;20,2,4,22;,
  4;20,22,4,2;,
  4;21,7,9,9;,
  4;21,9,31,30;,
  4;22,4,32,6;,
  4;22,6,6,4;,
  4;9,9,11,11;,
  4;9,11,11,31;,
  4;6,32,8,8;,
  4;6,8,8,6;,
  4;11,11,13,23;,
  4;11,23,23,11;,
  4;8,8,10,24;,
  4;8,24,10,8;,
  4;23,13,15,25;,
  4;23,25,25,23;,
  4;24,10,12,12;,
  4;24,12,12,10;,
  4;25,15,17,17;,
  4;25,17,17,25;,
  4;12,12,14,14;,
  4;12,14,14,12;,
  4;17,17,16,26;,
  4;17,26,16,17;,
  4;14,14,16,26;,
  4;14,26,16,14;,
  3;33,35,34;,
  3;33,36,35;,
  3;33,37,36;,
  3;33,38,37;,
  3;33,39,38;,
  3;33,40,39;,
  3;33,41,40;,
  3;33,42,41;,
  3;33,43,42;,
  3;33,44,43;,
  3;33,45,44;,
  3;33,46,45;,
  3;33,47,46;,
  3;33,48,47;,
  3;33,49,48;,
  3;33,34,49;,
  3;66,50,51;,
  3;66,51,52;,
  3;66,52,53;,
  3;66,53,54;,
  3;66,54,55;,
  3;66,55,56;,
  3;66,56,57;,
  3;66,57,58;,
  3;66,58,59;,
  3;66,59,60;,
  3;66,60,61;,
  3;66,61,62;,
  3;66,62,63;,
  3;66,63,64;,
  3;66,64,65;,
  3;66,65,50;,
  4;83,84,147,148;,
  4;83,148,149,85;,
  4;84,86,150,147;,
  4;85,149,151,87;,
  4;86,88,152,150;,
  4;87,151,153,89;,
  4;88,90,154,152;,
  4;89,153,155,91;,
  4;90,92,156,154;,
  4;91,155,157,93;,
  4;92,94,158,156;,
  4;93,157,159,95;,
  4;94,96,160,158;,
  4;95,159,161,97;,
  4;96,98,162,160;,
  4;97,161,162,98;,
  4;99,163,164,100;,
  4;99,101,165,163;,
  4;100,164,166,102;,
  4;101,103,167,165;,
  4;102,166,168,104;,
  4;103,105,169,167;,
  4;104,168,170,106;,
  4;105,107,171,169;,
  4;106,170,172,108;,
  4;107,109,173,171;,
  4;108,172,174,110;,
  4;109,111,175,173;,
  4;110,174,176,112;,
  4;111,113,177,175;,
  4;112,176,178,114;,
  4;113,114,178,177;,
  4;115,69,67,116;,
  4;115,116,84,83;,
  4;115,83,85,117;,
  4;115,117,71,69;,
  4;116,67,68,118;,
  4;116,118,86,84;,
  4;117,85,87,119;,
  4;117,119,73,71;,
  4;118,68,70,120;,
  4;118,120,88,86;,
  4;119,87,89,121;,
  4;119,121,75,73;,
  4;120,70,72,122;,
  4;120,122,90,88;,
  4;121,89,91,123;,
  4;121,123,77,75;,
  4;122,72,74,124;,
  4;122,124,92,90;,
  4;123,91,93,125;,
  4;123,125,79,77;,
  4;124,74,76,126;,
  4;124,126,94,92;,
  4;125,93,95,127;,
  4;125,127,81,79;,
  4;126,76,78,128;,
  4;126,128,96,94;,
  4;127,95,97,129;,
  4;127,129,82,81;,
  4;128,78,80,130;,
  4;128,130,98,96;,
  4;129,97,98,130;,
  4;129,130,80,82;,
  4;131,99,100,132;,
  4;131,132,67,69;,
  4;131,69,71,133;,
  4;131,133,101,99;,
  4;132,100,102,134;,
  4;132,134,68,67;,
  4;133,71,73,135;,
  4;133,135,103,101;,
  4;134,102,104,136;,
  4;134,136,70,68;,
  4;135,73,75,137;,
  4;135,137,105,103;,
  4;136,104,106,138;,
  4;136,138,72,70;,
  4;137,75,77,139;,
  4;137,139,107,105;,
  4;138,106,108,140;,
  4;138,140,74,72;,
  4;139,77,79,141;,
  4;139,141,109,107;,
  4;140,108,110,142;,
  4;140,142,76,74;,
  4;141,79,81,143;,
  4;141,143,111,109;,
  4;142,110,112,144;,
  4;142,144,78,76;,
  4;143,81,82,145;,
  4;143,145,113,111;,
  4;144,112,114,146;,
  4;144,146,80,78;,
  4;145,82,80,146;,
  4;145,146,114,113;;
 }
 MeshTextureCoords {
  292;
  0.500000;0.353413;,
  0.662010;0.746103;,
  0.500007;0.778460;,
  0.799348;0.653961;,
  0.891114;0.516064;,
  0.923335;0.353405;,
  0.891108;0.190747;,
  0.799338;0.052854;,
  0.661997;-0.039283;,
  0.499993;-0.071635;,
  0.337990;-0.039278;,
  0.200652;0.052864;,
  0.108886;0.190761;,
  0.076665;0.353420;,
  0.108892;0.516078;,
  0.200662;0.653972;,
  0.338003;0.746108;,
  0.500000;0.499978;,
  0.500007;0.925026;,
  0.662010;0.892668;,
  0.799348;0.800526;,
  0.891114;0.662630;,
  0.923335;0.499971;,
  0.891108;0.337313;,
  0.799338;0.199419;,
  0.661997;0.107282;,
  0.499993;0.074930;,
  0.337990;0.107288;,
  0.200652;0.199429;,
  0.108886;0.337326;,
  0.076665;0.499985;,
  0.108892;0.662643;,
  0.200662;0.800537;,
  0.338003;0.892674;,
  0.937500;0.239670;,
  0.937500;0.000000;,
  1.000000;0.000000;,
  1.000000;0.239670;,
  0.799160;-2.055300;,
  0.727800;-2.055300;,
  0.727800;-2.860610;,
  0.799160;-2.860610;,
  0.000000;0.239670;,
  0.000000;0.000000;,
  0.062500;0.000000;,
  0.062500;0.239670;,
  0.621750;-2.055300;,
  0.621750;-2.860610;,
  0.125000;0.000000;,
  0.125000;0.239670;,
  0.497160;-2.055300;,
  0.497160;-2.860610;,
  0.731810;-2.055300;,
  0.731810;-2.860610;,
  0.626990;-2.860610;,
  0.626990;-2.055300;,
  0.373010;-2.055300;,
  0.373010;-2.860610;,
  0.502840;-2.860610;,
  0.502840;-2.055300;,
  0.268190;-2.055300;,
  0.268190;-2.860610;,
  0.378250;-2.860610;,
  0.378250;-2.055300;,
  0.625000;0.239670;,
  0.562500;0.239670;,
  0.562500;0.000000;,
  0.625000;0.000000;,
  0.272200;-2.860610;,
  0.272200;-2.055300;,
  0.500000;0.239670;,
  0.500000;0.000000;,
  0.200840;-2.860610;,
  0.200840;-2.055300;,
  0.437500;0.239670;,
  0.437500;0.000000;,
  0.000000;0.777000;,
  0.062500;0.777000;,
  0.500000;0.170130;,
  0.624370;0.170130;,
  0.624370;0.919430;,
  0.500000;0.919430;,
  0.375630;0.919430;,
  0.375630;0.170130;,
  1.000000;0.777000;,
  0.937500;0.777000;,
  0.125000;0.777000;,
  0.729810;0.170130;,
  0.729810;0.919430;,
  0.270190;0.919430;,
  0.270190;0.170130;,
  0.875000;0.777000;,
  0.875000;0.239670;,
  0.187500;0.239670;,
  0.187500;0.777000;,
  0.731810;-0.249810;,
  0.626990;-0.249810;,
  0.626990;0.499490;,
  0.731810;0.499490;,
  0.199740;0.919430;,
  0.199740;0.170130;,
  0.812500;0.777000;,
  0.812500;0.239670;,
  0.250000;0.239670;,
  0.250000;0.777000;,
  0.502840;-0.249810;,
  0.502840;0.499490;,
  0.621750;-0.249810;,
  0.621750;0.499490;,
  0.497160;0.499490;,
  0.497160;-0.249810;,
  0.750000;0.777000;,
  0.750000;0.239670;,
  0.312500;0.239670;,
  0.312500;0.777000;,
  0.378250;-0.249810;,
  0.378250;0.499490;,
  0.373010;0.499490;,
  0.373010;-0.249810;,
  0.687500;0.777000;,
  0.687500;0.239670;,
  0.375000;0.239670;,
  0.375000;0.777000;,
  0.272200;-0.249810;,
  0.272200;0.499490;,
  0.199740;0.170130;,
  0.199740;0.919430;,
  0.270190;0.919430;,
  0.270190;0.170130;,
  0.625000;0.777000;,
  0.437500;0.777000;,
  0.729810;0.170130;,
  0.624370;0.170130;,
  0.624370;0.919430;,
  0.729810;0.919430;,
  0.375630;0.919430;,
  0.375630;0.170130;,
  0.562500;0.777000;,
  0.500000;0.777000;,
  0.500000;0.170130;,
  0.500000;0.919430;,
  0.031250;0.000000;,
  0.062500;0.000000;,
  0.000000;0.000000;,
  0.093750;0.000000;,
  0.125000;0.000000;,
  0.156250;0.000000;,
  0.187500;0.000000;,
  0.218750;0.000000;,
  0.250000;0.000000;,
  0.281250;0.000000;,
  0.312500;0.000000;,
  0.343750;0.000000;,
  0.375000;0.000000;,
  0.406250;0.000000;,
  0.437500;0.000000;,
  0.468750;0.000000;,
  0.500000;0.000000;,
  0.531250;0.000000;,
  0.562500;0.000000;,
  0.593750;0.000000;,
  0.625000;0.000000;,
  0.656250;0.000000;,
  0.687500;0.000000;,
  0.718750;0.000000;,
  0.750000;0.000000;,
  0.781250;0.000000;,
  0.812500;0.000000;,
  0.843750;0.000000;,
  0.875000;0.000000;,
  0.906250;0.000000;,
  0.937500;0.000000;,
  0.968750;0.000000;,
  1.000000;0.000000;,
  0.031250;1.000000;,
  0.000000;1.000000;,
  0.062500;1.000000;,
  0.093750;1.000000;,
  0.125000;1.000000;,
  0.156250;1.000000;,
  0.187500;1.000000;,
  0.218750;1.000000;,
  0.250000;1.000000;,
  0.281250;1.000000;,
  0.312500;1.000000;,
  0.343750;1.000000;,
  0.375000;1.000000;,
  0.406250;1.000000;,
  0.437500;1.000000;,
  0.468750;1.000000;,
  0.500000;1.000000;,
  0.531250;1.000000;,
  0.562500;1.000000;,
  0.593750;1.000000;,
  0.625000;1.000000;,
  0.656250;1.000000;,
  0.687500;1.000000;,
  0.718750;1.000000;,
  0.750000;1.000000;,
  0.781250;1.000000;,
  0.812500;1.000000;,
  0.843750;1.000000;,
  0.875000;1.000000;,
  0.906250;1.000000;,
  0.937500;1.000000;,
  0.968750;1.000000;,
  1.000000;1.000000;,
  0.875000;0.769250;,
  0.937500;0.769250;,
  0.812500;0.769250;,
  1.000000;0.769250;,
  0.750000;0.769250;,
  0.000000;0.769250;,
  0.062500;0.769250;,
  0.687500;0.769250;,
  0.125000;0.769250;,
  0.625000;0.769250;,
  0.187500;0.769250;,
  0.562500;0.769250;,
  0.250000;0.769250;,
  0.500000;0.769250;,
  0.312500;0.769250;,
  0.437500;0.769250;,
  0.375000;0.769250;,
  0.875000;0.180780;,
  0.937500;0.180780;,
  0.812500;0.180780;,
  1.000000;0.180780;,
  0.750000;0.180780;,
  0.000000;0.180780;,
  0.062500;0.180780;,
  0.687500;0.180780;,
  0.125000;0.180780;,
  0.625000;0.180780;,
  0.187500;0.180780;,
  0.562500;0.180780;,
  0.250000;0.180780;,
  0.500000;0.180780;,
  0.312500;0.180780;,
  0.437500;0.180780;,
  0.375000;0.180780;,
  0.875000;0.605390;,
  0.875000;0.436390;,
  0.937500;0.436390;,
  0.937500;0.605390;,
  0.812500;0.605390;,
  0.812500;0.436390;,
  1.000000;0.436390;,
  1.000000;0.605390;,
  0.750000;0.605390;,
  0.750000;0.436390;,
  0.000000;0.605390;,
  0.000000;0.436390;,
  0.062500;0.436390;,
  0.062500;0.605390;,
  0.687500;0.605390;,
  0.687500;0.436390;,
  0.125000;0.436390;,
  0.125000;0.605390;,
  0.625000;0.605390;,
  0.625000;0.436390;,
  0.187500;0.436390;,
  0.187500;0.605390;,
  0.562500;0.605390;,
  0.562500;0.436390;,
  0.250000;0.436390;,
  0.250000;0.605390;,
  0.500000;0.605390;,
  0.500000;0.436390;,
  0.312500;0.436390;,
  0.312500;0.605390;,
  0.437500;0.605390;,
  0.437500;0.436390;,
  0.375000;0.436390;,
  0.375000;0.605390;,
  0.875000;0.304440;,
  0.937500;0.304440;,
  0.812500;0.304440;,
  1.000000;0.304440;,
  0.750000;0.304440;,
  0.000000;0.304440;,
  0.062500;0.304440;,
  0.687500;0.304440;,
  0.125000;0.304440;,
  0.625000;0.304440;,
  0.187500;0.304440;,
  0.562500;0.304440;,
  0.250000;0.304440;,
  0.500000;0.304440;,
  0.312500;0.304440;,
  0.437500;0.304440;,
  0.375000;0.304440;;
 }
}