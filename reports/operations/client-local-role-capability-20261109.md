## 1. 작업 목표
서버표시파트를 제거하기 전에 현재 원본 자산의 로컬 생성 파트가 기존 DGNativeCommand/PreviewRequest 경로로 재생되는지 작은 격리 대조를 수행한다. 기존5파트는 유지한다. 정상밀침·낙하·착지·추적·재등반 코드를 변경하지 않는다.

## 2. 실제 수행
기존 단일5파트 고정Run·카메라·리테일 실행기와 서명 빌드 절차를 재사용했다. 서버5파트는 동일 자산/LOD0/요청2/rootMotion=false를 유지한다. 클라이언트에 같은 candidate1 파트를 ECE_LOCAL|ECE_CREATEPHYSICS로1개 추가하고 PreviewRequest=2, rootMotion=false, LOD0을 설정했다. 이 조건만으로 기존 자동명령 초기화가 성립하는지 본다. 명시적 StartCommand 호출·시계주입·후킹·서버파트제거는 없다.
서명/검증 성공한 진단PBO1과 개체0 컴파일1 뒤 network1을 실행했지만 buffered-only 수집기가 종료 전 저장하지 못했다. 유효포즈자료0으로 제외하고 원문/로그/failure1.json을 보존했다. 로컬 재생 실패로 세지 않았다.
build2는 관찰기 초기화 후 GetPlayer 반복guard를 제거하고, 진행 heartbeat/부분파일저장·GetTickTime 기반20초 종료를 추가했다. 서버는 수집종료 신호 또는100초까지 유지한다. 준비 코드를 고친 별도 build2/compile2/network2이며 실패를 덮어쓰지 않았다. 로컬 애니메이션 제어/자산은 바꾸지 않았다.

## 3. 측정 결과
network2 고정Run의1000표본 모두 appActive=true였다. raw callbackdt 합20.005초, native tick-time 종료경계 약20초, host 제어20.18초였다. observer Age 필드는 전부0이므로 그것으로 경과시간/갱신간격p50/p95를 계산하지 않았다. 이필드의 원인은 미특정이며 원시값을 보존한다.
기준파트3본 중 하나 이상이1e−5m를 넘게 바뀐 인접표본은998/999였다. 최초표본 대비 최대변화는 Head0.507492m/Hips0.062124m/RightHand0.557551m였다. 로컬3본은1000표본 전체에서 최대변화0m, 애니메이션/물리콜백 카운터 최대0/0, NativeRequest는0, PreviewRequest는2였다.
따라서 움직이는 활성 기준표본 gate는 통과했고, 로컬 정지 조건으로 기존자동생성 경로의 capability gate를 실패했다. 시작위상이 다르므로 동시포즈차로 판정한 것이 아니다.
로컬 GetCommand_Script 분리 확인 후삭제 경로가 detached=true로 반환했다. 서버5파트도퇴역분리 후삭제했고 actualexit−1·강제종료없음, 소유클라이언트는일반창닫기로종료했다. native 내부누수없음의증명은아니다.
새CPU/Present·8/64기능·14클립·실제화면부드러움 측정없음. 스크린샷에서는완성기준조립체와원래부분메시로컬파트의표시를보았으나시간적품질을판정하지않는다.

## 4. 원인 분석
확인: 현재멀티플레이클라이언트에서 로컬객체생성/본핸들조회/요청값쓰기만으로 기존커스텀명령이구동되지않았다. 클라이언트로그에 해당 DG_NATIVE_COMMAND_BOUND 마커가 없고 콜백카운터/본좌표도정지했다. 네이티브명령자동초기화 경로의 역할차이가 유력한 경계다.
미확인: 엔진이 이로컬객체의 명령handler를 왜 호출하지 않았는지, 명시적지원StartCommand_Script가클라이언트에서 동작하는지, physicsbody/authority/graph인스턴스설정이 어느부분에관여하는지. 이번수집기는생성직후모든native상태를기록하지않았으므로원인을더좁혀단정하지않는다.
판정: 현재생성패턴을그대로옮겨 서버파트를없애는 방식은미수용. 모든클라이언트로컬애니메이션이불가능하다고결론내리지않는다. 파일헤더분석이나서버시계동등성도이경계를대신하지않는다.

## 5. 검증 수준
PBO빌드·서명/검증·개체0 Enforce로딩:성공. network1:수집실패제외. network2:유효기준대조하의자동로컬경로기각 진단성공. 기능회귀·시각수용·성능개선·독립반복·최종수용:미실행/미수용.8→64확대하지않는다.

## 6. 변경 사항
격리 client-local-role-probe1에기본OFF진단PBO2개와각소스·미션·manifest·기존launcher사본·분석만추가했다. 기존10개입력PBO와게임플레이소스/모델/ANM불변을해시확인했다. 두진단PBO서명및DSCheckSignatures성공,개인키복사/배포포함0. build2초기빌드는직전종료중인런처guard로pack전에중단됐고,프로세스부재확인후같은빌드를실행했다. 강제종료/보안설정변경없음.
서버감소LOD 치료는실행하지않았으며종료시에원래LOD0 재설정2회만발생했다. 실제exit−1을exit0정상종료로바꾸지않는다.

## 7. 원시 증거
ANIM-client-local-role-compile1/network1/compile2/network2. build receipts/signature logs/source hashes, trial-contract1.json, build2/collector-change-contract.json, network1/failure1.json, network2 nf-local-role.tsv/heartbeat/cleanup, role-server-pose.tsv/role-stage.tsv, controller/server-terminal/client-closed를연결한다. 공개에는집계·선정해시만담고원본자산/내부경로/PID/플레이어ID는내보내지않는다.

## 8. 잔여 문제
이번자동연결후보는중단하고현행5파트를유지한다. 다음은실행을바로반복하기보다설치스크립트의명령초기화/클라이언트역할계약과공개StartCommand_Script의호출조건을먼저확인한다. 명시적명령초기화가검토할가치가있으면기존생성조건에그요인하나만추가한별도capability시험으로제안하며,기존0콜백자료를성능대조로재사용하지않는다. 지원경로가없으면로컬표현전환을구현하지않는다. 기존8/64기능시험이나등반복구개발로범위를늘리지않는다.64전체CPU와20ms p95목표는아직미해결이다.
