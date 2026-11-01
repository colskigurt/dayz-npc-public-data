# 그려지는 HUD 표식 확인·반복 이미지 coverage 재판정·외부 호드 자료 검토

작성: 2026-11-01T19:04:25.372467+09:00

## 1. 작업 목표

계측용 HUD binary 막대를 실제로 칠하고 checksum/client log와 연결한다. 동작/갱신 정책을 유지해 레이아웃 변경을 분리하며, 반복 캡처 이미지를 독립 pose 표본으로 세지 않는다. 중간에 사용자가 전달한6개 대량호드 참고자료는 설계후보로 분리 검토하고 현재 구현에 섞지 않는다.

## 2. 실제 수행

설치된 GUI style XML을 파싱해 Colorable 소유 위젯을 조사했다. Window/Button/GridSpacer 등에 정의가 있고 Panel에는 없었다. 이전 Panel에style만 추가한 준비v4는 실행하지 않고 보존했다. 별도후보는 root WindowWidget/24개ButtonWidget에 명시Colorable을 사용한다. script는이전v3와동일SHA fe9d763994e870157f367907b40f7aed3907ab8dbf315f2fd87e105350b2cfb5다. build/pbo source일치·서명검증을 마쳤다.

동일기본1part·고정run·network 진단에서2회시험했다. 시도1은녹화초반 검은화면이었고 활성화뒤구간에서막대를확인했다. 원본을보존했다. 시도2는실제foreground gate가준비돼야recorder가시작되도록 순서를수정했다. CU활성화실패뒤 실제창/프로세스를재확인해한번재시도했고성공했다. 새게임을관찰timeout만으로재시작하지않았다. 이후사용자메시지로turn이중단됐으나회수시두실행의client/server/recorder가모두종료됨을실제상태로확인했다.

영상해독첫crop은YUV의홀수높이반올림으로shape오류가났다. exact crop으로고쳤고원래생산자를보존했다. 고정120프레임geometry검사는시도1의검은화면이라거부됐다. 실제후반600프레임에서검은root사각형만으로scale/좌표를구한별도revision을사용했다. 프레임번호/native시계는geometry보정에쓰지않았다. 각24bit의16bit ID/8bit XOR checksum·밝기·로그멤버십·단조성을검사했다.

## 3. 측정 결과

두영상각723프레임이며OBS출력표식/QPC는각723/723연결됐다. 시도1 gameHUD209/723 valid·서로다른ID162개·양쪽TickCount native bracket이직접일치한ID30개다. native776표본/15.5036303초·TickCount bracket1553회검증,오류0이다. 시도2는gameHUD670/723 valid지만서로다른ID169개·직접bracket ID156개에그쳤다. native156표본/8.0785038초·bracket312회검증,이후callback2초공백으로관찰이끝났다. source-run/coverage차를보존하며15초완주로세지않는다.

단순해독이미지수>500의시도2 flag는true였지만이값을시간coverage수용으로쓰지않았다. 같은ID최대144이미지·반복501쌍이포함됐기때문이다. 독립ID/직접bracket 기준250개로다시검사해두실행모두temporalCoverage=false로기록했다. 이전flag도삭제하지않았다. 시도1반복47쌍중body/background ROI전체동일2쌍, 시도2반복501쌍중동일11쌍이다. 다른픽셀변화는바람/TAA/표시등을포함하므로본pose갱신증명도아니다.

scheduler callback간격p50/p95/p99는시도1 20.1018/22.86221/23.356104ms,시도2 52.1743/56.80475/60.129414ms다. 계측·focus·capture/lifetime조건이달라성능A/B로쓰지않는다. 실제game/display frame-time·8/64기능회귀·GPU/할당/GC는미측정이다.20ms p95는여전히목표다.

실행1영상44163058bytes/SHA87a5b90c3142cde032e8976c5b6f0cd96157eac7efd071e5c13ff02d2b8de789,실행2영상93043181bytes/SHA1bf67efc795621c7fc8360b851c351342728a9705ab98a4d08f3f91b18275850이다. 둘다CRF12·12초이며원시증거를보존하고추가full영상복사는없다. 두client정상종료marker·강제client종료없음·VM/crash0,serverDONE뒤소유server정리,recorder정상stop·basic/scene/encoder3설정원래bytes복원을확인했다.

## 4. 원인 분석

확인된사실: 등록된painted위젯을쓰는레이아웃에서는binary막대가보인다. 다만widget class와style를함께바꿨으므로style단독원인까지분리한결과는아니다. 계측script/animation동작은같다.

확인된한계: 체크섬통과이미지수만으로시간적관측량을판정하면같은HUD상태재표시를새관측으로오인한다. log/state ID가같으면서다른ROI픽셀이변하는경우도있어'동일HUD=전체게임프레임정지'로단정할수없다. callback/HUD유효기간·WGC비동기texture·UI갱신시점의관계는미확정이다. 사용자turn중단을엔진원인으로단정하지않는다.

다음계측에는실제foreground/HUD준비후남은fixture유효기간이native15초및독립ID250개를수집하기에충분한지확인해야한다. 부족하면녹화/attachment를시작하지않아야한다. 이번두실행의coverage를성공으로낮춰정의하지않는다.

## 5. 검증 수준

진단성공: 막대실제표시·checksum/log decode·기존QPC연결,반복표본에의한coverage오판방지. 미완료: 충분한독립시간coverage,render pose identity·texture age·physical scanout,최종애니메이션시각품질. 전체NPC회귀/반복시각검증/성능목표/최종수용은아니다. 기존mode0손실·변환경로·64개체비용실험을대체하지않는다.

## 6. 변경 사항

새계측PBO 1B130510D7C1A212384E41BD2A7984BAD1C226D74CABAC185B2C00C090A706DD는packed script/layout 일치및서명·검증성공이다. 개인키는release에없다. 기존453D/0C6E/B8FB 세게임PBO는변경없고각실행4개signature검증은성공이다. gameplay코드·ANM·운영서버·Steam배포는변경하지않았다. 기존준비v4/실패decode/black영상도보존했다.

## 7. 원시 증거

묶음 animation-diag-painted-label-20261101-1850의style-investigation/plan/source/build/packed proof/signature, runtime및runtime-2의launch/profile/native/heartbeat/callback-gap/marker/decode/geometry/hud-native-joined/recorder와analysis를보존한다. source/public해시검증·JSON파싱·종료확인후기존MD/TXT/checkpoint/진행JSON/B안에연결한다. 공개본은집계·선정geometry/대표join·한계만노출하며raw영상·실제networkID·PID·CLI·로컬절대경로·키·게임자산은제외한다.

## 8. 잔여 문제

우선현재capture의남은유효기간gate와충분한독립frame/native대응을확보한다. 새시간보정·보간·frame skipping을적용하지않는다. 이후pose/world/projection·실제이미지를동일ID에서대조한다.64개체비용최적화는별도통제실험이다.

사용자가제공한6개호드자료의공식listing/README를이번에확인했다. 소스전체검토·데모실행·구매/설치는하지않았다. [SwarmZ](https://www.fab.com/listings/b4f706c9-125d-493a-9620-db5b0b0ee980)는근거리skeletal/원거리GPU·baked animation과통합행동명세참고다. [Pioneer](https://www.fab.com/listings/463bd9f2-766c-4e09-8169-8e9ce1b781ac)는Actor-Mass연계·VAT/LOD전환경계참고다. 제작자수천개체수치를우리full NPC64개와동일정밀도로비교하지않는다.

[MiHordeTraffic](https://github.com/innocentmiau/MiHordeTraffic)는shared flow field·혼잡비용·spatial hash의공개연구후보다. README의5천개체18.68/12.08ms는editor/Collections checks조건이며우리성능근거가아니다. settling/경사거부/문대기는보존할낙하·재등반행동과충돌여부를검토해야한다. GitHub MIT표시는확인했으나이번turn에서라이선스파일/전체코드를다읽었다고하지않는다.

[crowd-simulation](https://github.com/gianmarcopicarella/crowd-simulation)은광장목표·일정속도NavMesh/회피UE5.3프로젝트로기존소스검토기록을먼저재사용한다. [MassZombies](https://github.com/stopthem/MassZombies)는UE5.2·단일AnimToTexture·static instances·hardcoded처치박스로표현/충돌타협을보는사례다. README의5600X/3060Ti25~30FPS는제작자관찰이다. [Massive Swarm System](https://assetstore.unity.com/packages/tools/game-toolkits/massive-swarm-system-384066)은store의가격/URP메타데이터를확인했으나추출된상세설명은비어기능검증을보류한다.

현DayZ모딩경로에서이들의Mass/Burst/GPU애니메이션·native multithread실행을직접연결하는경로는확보되지않았다. 이를불가능증명으로확대하지않고기능·API·비용검증미확보로표시한다. 신규검토순서는공유경로/혼잡소스(MiHordeTraffic),통합행동/근원거리전환명세(SwarmZ/Pioneer),표현비용사례(MassZombies)이며현재애니메이션진단의선회를뜻하지않는다.

선정 공개 본문. 로컬 보고서 SHA-256: `e229e74b669b2e0eaf1953c9619d8802a537c362036bf2f4882cb613309db3b0`. 원시 전체가 아니다.
