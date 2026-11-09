# 스키닝 셰이더 용량 — 255행렬 선언과 미확인 매핑 경계

## 1. 작업 목표
파트별 네이티브 호스트 비용을 줄이는 구조 연구에서,64가중본 예산이GPU셰이더의확정한계인지검사한다. 기존5파트/4파트결과와전체모델변형실패를보존하고,근거없이같은8→1합체시험을반복하지않는다.

## 2. 실제 수행
기존native-skin-variants-result/20261023 client결과/48본분할작성코드/palette-budget판정/4파트산술하한을읽었다. 현행설치system.pbo를기존PBOreader로읽고,$SKIN 이름의vertex shader118개를Windows에설치된D3DDisassemble로정적해독했다. 게임/셰이더실행·프로세스후킹·네이티브메모리수정없음. REA스킬의artifact/evidence경계를적용했고,광고된REA도구에shader전용decoder가없어기존로컬형식reader와시스템decoder를사용했다. REA/Ghidra전체초기화는시작하지않았다.
각DXBC의총길이/청크범위를검증하고SHA를기록했다. 선택shader하나의해독문만로컬보존했다. 기존SDK추출118파일과바이트해시비교했고,기존전체/sectioned/part P3D5개의ODOL헤더를읽었다. 현재client해시를고정하고세개상수이름의ASCII/UTF16문자열존재만제한검색했다.

## 3. 측정 결과
- 실제설치$SKINvertex shader118/118: CBufferSkinMatrix에float4x3행렬255개,12240byte선언. 선택shader는CB5[765]를dynamicIndexed로선언하며입력blend indices를변환해행렬행을참조한다.
- 기존SDK추출본118/118은현재shader와동일SHA. 이는해당옛실행의실제사용shader/추출시각을인증하는것은아니다.
- 기존full/sectioned/분할모델5개는이미ODOLv56이다. 단순binarization이누락됐다는설명은선정모델에맞지않는다.
- 현재client는e3ba9660…고정,파일버전1.30.164.289. 세상수이름의PE문자열검색은ASCII/UTF16모두0. 문자열부재는업로드함수부재증거가아니다.
- 새게임·PBO·8/64회귀·CPU/Present p50/p95/p99·시각품질측정0.

## 4. 원인 분석
확인: 조사한현재shader의정적행렬버퍼는64개가아니라255개다. 따라서64가GPU셰이더전체의확정행렬상한이라는설명은이자료로뒷받침되지않는다. 기존4파트최소성은64예산을가정한정적배치의조건부하한이며,엔진모든경로의최소파트수증명이아니다.
보존한실패: 단일전체P3D와48가중본/텍스처구간분할은과거시각변형실패. 제한된본을가진별도모델은개선됐고64본파트도후속검증됐다. 이번선언발견으로과거실패를성공으로바꾸거나한파트구현가능성을확정하지않는다.
미확인: 실제선택draw의shader와상수버퍼내용,로컬/글로벌boneindex매핑,팔레트구축·업로드의상한/오프셋,엔티티별CPU호스트가한모델을처리할수있는조건. 후보원인이shader배열크기외의메시변환/업로드/매핑경로에있을가능성을남긴다.
공식1.30변경이력은250global bone제한제거와이름기반index를서술한다. 이것은모델별weighted palette/렌더upload한계와구분해야한다. 이항목으로단일host수용을선언하지않는다. [공식변경이력](https://dayz.com/article/game-update/1-30-experimental-release/dayz-steam-patch-notes).

## 5. 검증 수준
설치된DXBC선언/청크/SHA교차검증진단성공. 실행shader선택·렌더업로드원인특정은미완료. 모델수정/새기능/실제단일host/CPU개선/시각수용/반복회귀/최종수용미실행.20ms목표미달성유지.

## 6. 변경 사항
기존work-reduction-review1/shader-palette-readonly1에선정정적증거/자체reader/요약을추가했다. 게임/모델/원본/PBO/그래프/지원판정/보안설정/운영서버불변. 시스템DLL을reader프로세스에서해독용으로호출했을뿐shader를실행하지않았다. 새서명대상없음. 원시shader/전체디스어셈블리/게임자산은공개하지않는다.

## 7. 원시 증거
ANIM-skin-shader-readonly1. shader-layouts1.json에system.pboSHA·decoderDLLSHA·118memberSHA와선언을기록했다. capacity-crosscheck1.json,existing-model-formats1.json,selected-shader-disassembly1.txt 및작성코드. 기존TRACK_B_NATIVE_CLIENT_RESULT_2026-10-23.json은rootCauseConfirmed=false를명시한다. 저장전원시증거해시를검증한다. 제한적PE검색을전체엔진분석으로승격하지않는다.

## 8. 잔여 문제
다음단일산출물은기존컴파일된전체ODOLv56와64본part의팔레트/본index매핑비교표다. 지원되는reader를확인하고양쪽모델이같은bone/weight의미를보존하는지부터판단한다. 이미실패한텍스처구간분할을이름만바꿔재시도하지않는다. 매핑/업로드의구체적가설이생기기전에는새모델변환·Workbench·8→64시험을시작하지않는다. 공개ODOLreader는[DayZObjectBuilder원본저장소](https://github.com/SXDIST/DayZObjectBuilder)의지원주장이있으나현재파일에대한정확성/구문검증은아직수행하지않았다. GPUcapture는현재범위에서수행하지않았고표준경로RenderDoc부재만확인했으며시스템전체미설치를단정하지않는다.
