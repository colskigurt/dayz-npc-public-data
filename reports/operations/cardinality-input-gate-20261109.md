# 골격 크기 분리 입력 — 48→248본 정적 계약·서명 통과

## 1. 작업 목표
단일전체모델실패의원인이P3D골격목록크기인지,많은가중면/구간/행렬업로드인지분리하기위해먼저통제가능한입력을준비한다. 기존전체합체실패/5파트안정기준을보존하고큰새모델재구성을시작하지않는다.

## 2. 실제 수행
기준PBO의실제config.bin을기존CfgConvert로읽어Animal_DGNativeRepartition0의상속과effective enFanimsys를확인했다. full root_motion XOB/native_traversal.agr/native_combat.asi가기존packed resource/로컬source와바이트일치했다. 전체TXO250node에모든P3D본이있고기준Memory48본위치는전체Memory의해당점과바이트동일했다.
처음249본full골격은부모관계248개가있어48본flat기준과비교하면두변수가바뀐다. 실제dg_flat의flat골격은248본이므로이를사용했다. 준비기에서틀린고정골격별칭/249가정을두차례거부했으며이때후보폴더/모델/PBO/게임은생성하지않았다. source유효성검사후48→248비교로최종계약을저장했다.
정상48본MLOD전체를바이트그대로격리후보에복사하고model.cfg의평면골격목록만248본으로지정했다. 차량클래스는기준클래스를상속하고model경로만바꾼다. 기존AddonBuilder/서명/검증helper를재사용해별도PBO를빌드했다. ODOLreader로실제결과와기준을비교했다. Workbench/native XOB/ANM변환·게임실행없음.

## 3. 측정 결과
- 원본MLOD모든LOD바이트동일SHA f9edad4c…;메모리점추가0.
- 컴파일골격본48→248확인,양쪽부모관계모두빈값. 가중본48/하위매핑표48/시각면20,230/시각정점13,494동일.
- 시각·Memory·物理3LOD의좌표/면/노멀/vertexflag/UV/텍스처/재질/속성과bounding center정확일치. 코너별본이름/수치가중치의canonical SHA도일치.
- 추가검사:3LOD의비어있지않은named selection은각48/48/1개. 이름·정점membership·수치가중치·face membership모두일치. 이48-entry표는구간wrap해석이필요없다.
- packed config의후보class는Animal_DGNativeRepartition0상속,model경로외변경없음.
- 새PBO SHA256 `E6630AE0A6B699EC8E1AF6DB0A3F2FC1B4115D56B41E865B0079459B0A22DE18`,서명/DSCheck검증성공,개인키미포함.
- 실제runtime/CPU pose/표시/8·64기능/Present p50/p95/p99·CPU개선미측정.

## 4. 원인 분석
확인: 이제시각/메모리/물리의컴파일결과와현재full native자산을유지하면서P3D골격목록크기만다른입력쌍을확보했다. 불필요한골격본추가시binarizer가48본으로자동가지치기한다는상황은이번결과에없다.
추정/시험목적: 같은48가중본·48하위표라도248P3D골격에서실제표시가달라지면weighted본개수외의골격이름매핑/크기/업로드경로를의심할수있다. 동일하게보이면P3D전체본수만으로과거전체모델실패를설명하기어려워진다. 어느결과도아직관측하지않았다.
미확인: 엔진runtime본바인딩/현재fullinstance포즈와실제draw. static속성동일을시각동일/인게임통과로바꾸지않는다. 신규후보는아직1host전체몸체나비용절감구현이아니다.

## 5. 검증 수준
packed설정/자산정체성·소스모든LOD동일·빌드·서명검증·컴파일ODOLsemantic관문통과. Enforce/game module실행·공통CPUpose·화면·기능·성능·최종수용미실행.20ms p95목표미달성유지.

## 6. 변경 사항
기존shader-palette-readonly1/cardinality-contract1에설정감사출력,cardinality-probe1에작은default-isolated데이터PBO와그입력/계약/빌드영수증만추가했다. 원본PBO/모델/Script/애니메이션/운영설정불변. 복사된MLOD는이번통제입력한개이며원본파일변경없음. 기존개인키는읽기전용사용했고배포물에넣지않았다. AI/물리검사/본갱신시계변경없음.

## 7. 원시 증거
ANIM-cardinality-contract1 / ANIM-cardinality-probe1-build / ANIM-cardinality-probe1-memory. audit1.json,trial-contract1/source-proof1,fixture-build/signature logs,compiled-contract1,memory-contract1,packed-config.cpp,입력/출력SHA. 실제본래PBO453d6a9e…는후속검사에서도일치. 공개는해시/집계/검증범위이며모델·원본애니메이션·개인키·로컬경로는제외한다.

## 8. 잔여 문제
다음은이입력쌍만사용한1개체쌍고정Run/idle의네트워크runtime검사다. 같은fullinstance/command/카메라에서공통CPU본좌표와active화면을비교하고safe retirement를확인한다. 게임에서fullinstance또는후보바인딩이성립하지않으면새보간/시계수정으로확대하지않는다. 표시해독이없으면CPUpose만으로통과시키지않는다. 결과가골격수원인가설을구분할때만수행하며14클립/1파트전체몸체/8→64비용시험으로자동확대하지않는다. 기존밀침·낙하·착지·FOLLOW·재등반통합코드는계속보존한다.
