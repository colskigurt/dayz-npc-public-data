# 네이티브 Move·기존 스크립트 root 전환 — 이동 경로 확인·시간 기준 위험 확인

## 1. 작업 목표
이전단독nativeMove능력확인후기존지상↔등반/낙하전환계약을검토한다. 현재모델의nativeMove와기존DGNativeCommand.ApplyRootMotion분기의실행·명령수명·시간기준을분리해확인한다. CPU개선이나전체NPC하이브리드수용시험이아니다.

## 2. 실제 수행
기존DGNativeProbe.RunPrePhys/RunPreAnim,Body.RenderOrigin/Drive,Presentation/미션Commit,Traversal.SetPhase/Settle을읽었다. native-ground-handoff-contract.md에단일좌표권한·원점/heading·Contacts/lift·롤백·수신체·시간·명령수명을정리했다. 기존Commit은delayed sample로렌더transform을매프레임덮어쓰며,기존script명령은자체Age를0부터시작한다.
기존follower-root branch-probe도재사용했다. 그실험의ApplyRootMotion=true개체에는GroundBody가있어먼저managed-zero분기를탔으므로unmanaged script-root실제이동을입증한것이아니었다. 이번은GroundBody없는단독동일모델에서기존지원분기를실행했다.
서명검증된nativeMove프로브PBO/원본13개를재사용하고미션만격리했다. nativeidle→nativewalk→scriptwalk→nativewalk/run→scriptrun→nativeidle,30게임초1회. script단계는NMPEnabled=false/PreviewRequest/ApplyRootMotion=true로원래DG명령을사용했다. native재진입은StartCommand_Move로엔진이교체/파괴를소유하며live command를수동삭제하지않았다.
최초미션은terrain-1의공백없는숫자뺄셈표기에서컴파일실패했다. 실패영수증보존후별도revision2에공백을고쳐실행했다. 새PBO/새서명은없다. 이전PBO해시/서명증거를재대조했다. 새ETW·영상·큰덤프·CPU벤치없음.

## 3. 측정 결과
단독현재single224 Animal1개,GroundBody/피해수신체/추적AI는연결하지않았다.472행, 0.001999~30.026게임초.
| 단계 | native/script | 관찰 구간(게임초) | 수평 순이동(m) | script root 적용량 증가(m) |
|---|---|---:|---:|---:|
| 0 idle | native | 0.002~2.990 | 0.0406 | 0.0000 |
| 1 walk | native | 3.051~7.971 | 9.8738 | 0.0000 |
| 2 walk | script | 8.048~12.947 | 9.8457 | 10.0881 |
| 3 walk | native | 13.007~15.950 | 6.2200 | 0.0000 |
| 4 run | native | 16.011~20.950 | 23.4350 | 0.0000 |
| 5 run | script | 21.026~25.967 | 24.2198 | 24.0357 |
| 6 idle | native | 26.029~30.026 | 0.0546 | 0.0000 |

script콜백PreAnim/PrePhys각198회,script root 적용누적 34.8568m. nativeMove콜백각406회·OnActivate3회. 두script구간에서root적용과실제수평이동이함께관찰됐다. 설정한소유권변경4회모두관찰됐고,표본에서최대일치지연0.061게임초.1개체생존,스크립트오류0,terrain보다1m아래로내려가는긴급중단표식없음. 이중단가드는지지판정허용오차/정상낙하수용기준이아니다.
21.147초에script Age진단값4.95→0.05초재시작이관찰됐다. native구간에는이필드가4.95에머물렀다. 이것은native graph시간이아니며graph재생시간초기화증거로바꾸지않는다.
GetCurrentAnimationInstanceUUID결과는모두0으로instance/재생연속성증거로사용하지않았다. 전환인접두표본의위치차0.085~0.587m/약0.060~0.077초. 큰전환차에는두physics콜백이포함됐다. 일반run에서도최대차 0.623m·표본속도 10.214m/s가있어숫자차만으로시각순간이동이나그부재를판정하지않는다. 이분석은사후서술이며새수용문턱이아니다.
CPU/Present p50/p95/p99·충돌/지지동등성·실제화면부드러움미측정. callback수를표시프레임수로해석하지않는다.

## 4. 원인 분석
확인: 기존DGNativeCommand의unmanaged ApplyRootMotion분기가현재모델에서엔진으로root를넘기고실제이동으로이어진다. nativeMove를계속교체하지않고기존애니메이션명령을유지할별도후보의능력근거다. 현재전체NPC는GroundBody분기가먼저실행되어이플래그만켜도같은결과가나오지않는다.
확인: 명령재생성은script Age/Owner.NativeSeconds의기준을초기화한다. 기존Traversal.NativePhaseAt/Settle의차분소비와결합하려면갱신순서·연속성·유효재기준이필요하다. 실제LAND/SETTLE시험은하지않았으므로착지가실패했다고보고하지않는다.
추정: 동일DG명령을유지하고지상root처리를엔진에넘기는방식은Move↔Script반복교체보다시간·수명위험을줄일수있다. 그러나기존transform Commit/논리readback/압력/Contacts·lift소유권과native충돌·지지의안전성은여전히해결되지않았다. 단순if한줄최적화나동등한물리교체로취급하지않는다.
미확인: 실제월드벽/발판가장자리/개체간접촉에서이경로의물리판정,피격·사망·수신체·낙하/재등반보존,전체CPU절감과64비용. rootAPI사용이nativequery를줄였거나더싼controller라는뜻은아니다.

## 5. 검증 수준
기존분기실행·실제이동·명령교체·script Age재시작:진단성공. full-NPC handoff/착지회귀·물리동등·시각·반복성능·최종수용:미실행. 명령연결성공을전체기능통과나CPU최적화성과로보고하지않는다.20ms p95는목표다.

## 6. 변경 사항
기존research/rate100-isolation-20261112안의전환계약과native-script-root-handoff1 격리미션·계약·로그/JSON/보고서만추가했다. 원본13PBO/게임/그래프/모델/운영서버/개인키/기존증거보존. 재사용프로브PBO SHA256 5d7184ef91b44ff4a4a5ffb0ccca918cf20b46a75324d24af78cabc49f651db8,이전서명·서명검증/packed소스일치증거와현재해시대조. 새빌드·재서명수행으로기록하지않는다.
성공시험서버retained exit−1/강제종료없음,클라이언트정상닫기. 컴파일실패서버종료는별도영수증. 현재시험프로세스소멸확인,원시로그/이전보고서덮어쓰기없음.

## 7. 원시 증거
실행ID NATIVE-SCRIPT-root-handoff1-r2. 같은폴더의contract.json,mission/init.c,server-launch/server-terminal/client-closed,runs/handoff/server-profile/native-script-handoff.tsv472행,analysis.json,transition-edge-review.json. 상위실패runs/handoff의로그/failed-terminal과native-ground-handoff-contract.md를연결한다. 기존PBO서명로그·후보source 및SDK관련증거는이전native-animal-move1자료를재사용한다.
공개는선정source/대표전환행/집계·해시이며전체설치자산·SDK PBO·로컬절대경로·프로세스정보·키제외. 정적계약과현재unmanaged프로브실행의검증범위를분리한다.

## 8. 잔여 문제
다음단일실험판단은지원되는script-root엔진이동의기본월드충돌·발판이탈판정이다. 기존fixture/동일현재모델의벽·가장자리작은조건에서실제정지/통과/수직반응을확인해야한다. 통과해도현재4관절솔버와완전동등하다고하지않으며실패하면지지/충돌삭제방향으로확대하지않는다. 이미확인한명령전환/단순평지이동만재시험하지않는다.
유망할때만단일DG명령을유지하는지상물리어댑터를검토한다. 현재전체Body의navigation/압력·논리위치/보간Commit/전환검사는그대로인상태에서바로채택할수없다. 기존전체NPC파이프라인을조용히교체하지않는다. 기능·시각·비용수용은별도이며8→64검증과애니메이션품질/20ms목표는남았다.
