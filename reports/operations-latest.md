## 1. 작업 목표

기존 상태 제한 SupportSweepFast 후보의 실제 wrapper 비용을 OFF/ON으로 비교한다. 전역Fast의 지형약6.6% overhead와 적격지붕효과를 구분하고, 정상지원/접촉/낙하 판정을 보존한다. 동시에 부모참조 입력의 분할코드를 전체의사코드와 대조한다.

## 2. 실제 수행

기존9개서명모듈과 native live-world/idle-pose 하네스를 재사용했다. 같은 미션·같은 PBO에서 SFPolicyRead=true 뒤 SFEnabled만 OFF/CONTEXT로 바꾸었다. globalFast/Shadow는둘다false다. 지형은ground/platformfalse, 나머지는high/FOLLOW/platformtrue 상태힌트로 후보의실제eligibility를통과시킨다. 실제 ray/sphere/법선/높이/footprint/joint 검사는그대로다. roof_minimumLift거부는지붕anchor에서0.3m이며옛지형거부조건과다르다.

독립서버프로세스2개를순차실행했다. 각각16group·4arm·1000call=64000solve, 총128000. 첫실행OFF/CONTEXT/CONTEXT/OFF,둘째역순이다. 측정전32warmup, 각group결과/이유/접촉점·lift범위검사, root/pose 고정, actual context/original counters를확인했다. 알려진Ghidra분석worker가종료된뒤에만측정했다. 클라이언트·새영상·새PBO는없다.

2시간deadline의공식REA stdio세션은1회초기화를재사용해3함수를decompile한뒤close_binary·serverexit0으로종료했다. 원본A500전후hash동일. Defaultanalyzer를생략하지않았다. 입력reader는interior c044f8에서canonical c035e0으로해결됐고, 앞서원본MSVCunwind7fragment연결과일치한다. caller/parentsetter b59610,공통serializer b5b5d0도성공했다. decompiler의타입전파/겹친로컬경고는보존한다.

## 3. 측정 결과

|실행/조건|OFF 중앙값 ticks/1000회|CONTEXT 중앙값 ticks/1000회|변화|
|---|---:|---:|---:|
|1/terrain|551648.5|554980.5|+0.604%|
|1/roof|769914.0|577763.5|-24.957%|
|1/edge|653558.5|569588.0|-12.848%|
|1/lift_reject|728941.0|536559.0|-26.392%|
|2/terrain|550268.0|558860.0|+1.561%|
|2/roof|774468.0|585679.5|-24.377%|
|2/edge|653214.5|577597.5|-11.576%|
|2/lift_reject|725632.0|539906.0|-25.595%|

위값은동일메서드1000회경과ticks의표본중앙값이다. 순수CPUself-time·게임/Present프레임ms가아니다. 런별메서드표본p50/p95/p99와두방향대조·질의counter는공개JSON에별도제공한다. 프레임p50/p95/p99는이번에없다. 지붕효과의양방향중앙값은첫실행-25.219/-25.160%,둘째-24.049/-25.022%다. 지형은후보비적격경로로contextCalls0을확인했고새래퍼검사overhead가남았다.

각실행64행·가드모두통과,OFFcontext0/original1000,적격지붕ONcontext1000/original0,누적footRays=10000-skippedRays,skippedRays=4*fastUsed 계약통과. accepted/pointCount·root/pose가비교내에서일치했다. 허용lift차이최대8mm가드는기존과같고모든조건완전동일lift라고하지않는다.

기존후속선별기준(지붕중앙/양방향각5%이상감소·지형중앙overhead3%이하)을두프로세스에서통과했다. 이는전체성능수용기준이아니다.8/64기능회귀·시각품질·malloc/GC·Present/CPUbusy/GPU·20msp95달성은이번미측정이다. 마지막8/64기능성공은기존support-context기록이며이번방법시험으로독립반복을늘리지않는다.서버퇴역detach후owned정리,exit-1·normalQuitVerifiedfalse.

## 4. 원인 분석

확인:기존전역Fast와달리상태래퍼는일반지형에서추가지형bound검사를피하고적격지붕의반복메서드비용감소를유지했다. 다만지형래퍼분기비용0.6~1.6%가있고전체프레임이25%빨라졌다는결론은아니다. Footray감소에는추가boundsphere가동반되므로총질의개수만의절감으로설명하지않는다. 위험은8mm범위lift변화·힌트stale·혼합전투·경계의일반화이며,실제물리가드를유지해도추가회귀가필요하다.

부모입력진단:전체reader에8byte ID초기화0과pivot-1이있고,후속presence비트가설정되면ID8byte·pivot4byte·bool을읽어b59610에전달한다. b59610은현재부모와resolved대상비교·기존연결해제·즉시연결또는pending등록을수행한다. 이흐름은기존실제caller와일치한다. 공통serializer의부모없음writer분기는ID0/pivot-1/bool0을저장한다. 그러나이함수가문제패킷의실제writer였다고관찰한것은아니며,wire비트와snapshot값은아직연결하지못했다.

초기화누락을확정할근거가없어해당가설을채택하지않는다. 모든임시로컬의liveness를의사코드명만으로증명하지않고어셈블리qword쓰기와대조한다. 서버송신오류·client정렬오류·engine/asset원인중하나를확정하지않는다. 원본코드를강제patch하거나부모ID0처리로경고를숨기지않았다.

## 5. 검증 수준

현재wrapper의고정입력메서드비용:2독립프로세스내짝비교반복검증,후속선별통과. 합성조건결과/포즈/위치가드:제한진단통과.3선택native함수:진단성공. 이번8/64게임플레이·실제시각품질·전체성능·최종수용:미실행/미수용. 전체골active.

## 6. 변경 사항

새PBO/게임알고리즘변경없음. 기존9PBOhash/서명검증을각실행전에통과했다. support후보1C879D1F와이전원본source/배포/Steam/asset/개인키/실패기록보존. 비교미션의실제바이트SHA=62f2fd5e2539c3a2b9316be73ea3ea308000a6a55dd3b0c26c537193c6565e66,두실행동일. 준비중LF정규화해시52c6...와실제CRLF파일해시의차이를실행전에발견해plan에둘다구분기록했다. 엔진컴파일·실제64행생성은이번에검증됐으며준비단계미실행기록을덮지않는다.

## 7. 원시 증거

context-method-cost1/2의plan/source/launch/signaturechecks·24열TSV·퇴역/완료RPT,context-method-cost-analysis와소스,preflight/runner문법기록을해시에연결한다. network-reader-batch1의공식광고toolcontracts/open/batch3결과/close/state·선택의사코드,원본unwind/parentlocalsJSON도연결한다. 공개는메서드집계·상대주소·흐름설명이며원시전체/게임decompile본문/개인키/프로세스/로컬경로/asset은제외한다.

## 8. 잔여 문제

다음비용산출물은같은현재8→64장면에서의OFF/ON전체프레임비교다. 이메서드결과만으로후보를전역활성화하지않는다. 기존정상밀침·낙하·착지·FOLLOW·재등반과역사적실패를유지하고기능/실제화면/성능을독립판정한다. 부모presence비트의실제생성자와reader직전bitcursor계약은별도로좁히되,매번초기화를다시하는툴실행을반복하지않는다. 결과에따라수정여부가달라지는최소질문부터확인한다.

로컬checkpointSHA256: 1323a49f6caf45c7c449f36d5af4a050c721b21d4a5608da90faf2e910ce48fa
