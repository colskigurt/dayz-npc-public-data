# 피해 수신체 Native Move 교체 진단 — 로컬 포즈 일치, 제자리 유지 실패

## 1. 작업 목표
현재 두 호스트를 유지하면서 피해 수신체의 script 명령 처리만 Native Move로 바꿀 가능성을 판단한다. 단일호스트통합/수신체골격축소와구분하며,기존표시·지지·충돌·부위피해를변경하기전root0계약을검증한다.

## 2. 실제 수행
기존SDK선언을재사용해AnimCommandBase와DayZAnimalCommandScript를대조했다. PrePhys_SetTranslation은script명령에선언되며NativeMove에같은root0인터페이스가있다고가정하지않았다. 기존NativeMove프로브는pose요청과speed를함께바꿨으므로새질문은pose1/2와speed0의분리다.
실제현재Animal_DGNativeDamageReceiver동일모델2개,GroundBody없음,script root0 대조와NativeMove후보,12게임초1회. 양쪽idle0~2초·walk2~6초·run6~10초·idle10~12초,NativeMove이동입력은계속0. 위치강제보정/새보간/물리검사삭제없음. 기존서명된프로브와13PBO를그대로재사용했다. 살아있는2개체의종료는기존StartCommand_Move handoff를시도했으나native crash로끝났다.
직전사망수명시험의기존0.8MB미니덤프와이번자동생성덤프를기존Windows판독기로읽었다. REA의Linux ELF크래시도구는이포맷에부적합해사용하지않았다. 기존PE/Capstone판독기를재사용해직전stack후보한개주변명령만확인했다. 새게임/전체Ghidra/후킹/메모리패치/추가덤프수집을분석목적으로시작하지않았다.

## 3. 측정 결과
실행 RECEIVER-native-move1, 독립1회, 성능비교쌍0회,378행/개체별189표본.2개체전구간생존·script오류0.

| 항목 | script root0 대조 | Native Move/speed0 |
|---|---:|---:|
| walk관측수평순이동 | 0m | 8.395686m |
| run관측수평순이동 | 0m | 18.961915m |
| 초기정착점대비활성구간최대수평거리 | 0m | 27.533278m |
| 마지막script PreAnim | 244 | 0 |
| 마지막Native PreAnim | 0 | 245 |

활성창의동시Head/Hips MS포즈최대차각0m. 로컬포즈일치와세계위치일치는별개다. 실행전1mm제자리/포즈gate중제자리조건실패. 이수치는전신14클립·실제피격·화면부드러움·CPU절감증명이아니다.
종료서버−1073741819(0xC0000005),Finish후native crash.클라이언트정상닫기/강제종료없음.현재모든시험프로세스종료. CPU·Present p50/p95/p99·8/64전체NPC회귀·시각품질미측정.

## 4. 원인 분석
확인: NativeMove에서이동입력0은요청된walk/run의root이동을억제하지않았다. 따라서기존root0수신체를직접대체하면피격몸체가표시몸체에서벗어날수있다. 위치를되돌리는새실행방식을붙여이를숨기지않는다.
확인: 이번은모두생존했는데도Finish에서접근위반이발생했다. 직전사망개체특유의오류로단정할수없다. 두덤프모두로드모듈밖주소의EXECUTE위반이며raw stack최상단후보RVA0x220989가같다. 해당직전명령은call[r8+8]이지만원래객체타입/잘못된값의생성자/원인함수는모른다. raw후보는정식unwind stack이아니다.
확인:2026-10-26의옛client모듈내RVA0x5378bc null READ와관측오류유형은다르다. 서로다른버전·서버/client이므로같은근본원인인지도모르며옛수정법을그대로재사용할근거가없다. 이번공통종료문제를현재8/64정상미션전체의새회귀로확대하지않는다.

## 5. 검증 수준
NativeMove변수제어·Head/Hips로컬포즈일치·speed0에서도root이동·기존덤프오류유형분리:진단성공. 제자리gate:실패. 정상종료/안전수명:실패. 실제NPC통합·성능/시각/반복수용:미실행·미수용.20ms p95는목표이며개선률없음.

## 6. 변경 사항
receiver-native-move1내격리미션/계약·로그·분석/판정만추가했다. 기존게임코드/PBO/모델/그래프/운영서버/원시기록보존. 프로브PBO SHA256 5d7184ef91b44ff4a4a5ffb0ccca918cf20b46a75324d24af78cabc49f651db8 및기존서명검증증거재사용. 새빌드/재서명없음. 기존소형Windows판독기와정적PE도구사용,설치/설정변경없음. 과거분석JSON은덮어쓰지않고추가분류로연결한다.

## 7. 원시 증거
contract.json,PRE_IMPLEMENTATION_REVIEW.md,init.c,receiver-native-move.tsv378행,analysis.json,disposition.json,server-terminal/client-closed,RPT/crash/기존mdmp,dump-summary.json. 직전single-host-death의dump-summary/dump-classification과예전shutdown FINDINGS를연결한다. 공개는집계·선정바이트/상대ID·해시로제한하고덤프/설치자산/프로세스정보/키/개인경로는제외한다. 기존원시와보고서는변경하지않았다.

## 8. 잔여 문제
수신체NativeMove직접교체는제자리조건이이미실패해종료크래시를고쳐도그대로채택할수없다. 이후보를살리기위한반복·위치보정·명령수명기능개발·64확대를하지않는다. 사망통합과단순수신체명령교체모두현재안전한전체CPU절감경로로입증되지않았다.
목표는계속물리/CPU실제비용감소다. 미세Ray캐시의0.010663%기회나로컬산술퍼센트·기능도착률을큰폭최적화로보고하지않는다. 현재큰폭CPU절감후보는미확보이며,기존단일렌더+수신체의정상밀침·낙하·착지·FOLLOW·재등반과사용자시각검증을보존한다.
