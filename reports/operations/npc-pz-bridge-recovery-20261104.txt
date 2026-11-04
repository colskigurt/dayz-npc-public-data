# 좀보이드 연결 복구·Days Gone 재사용 조사

작성: 2026-11-04T15:29:34.826414+09:00

## 1. 작업 목표

좀보이드–DayZ 연결 작업의 상세 기록과 자체 어댑터 코드를 Steam 비공개 보관본에서 별도 경로로 복구한다. 기존 구현을 덮어쓰거나 실행하지 않고 Days Gone–DayZ 결합에 재사용할 부분과 신규 의존성을 판정한다. 현재 64개체 최적화 연구는 보존한다.

## 2. 실제 수행

고정 Recovery 다운로드 index SHA, manifest·transport SHA와 필요한 ZIP 3개 전체 SHA를 대조했다. 보관 PassthroughLab의 문서·자체 코드·작은 결과 JSON 221파일, 1,613,716bytes를 새 research/pz-bridge-recovery-review-20261104/recovered 경로에만 복구했다. 221개 파일의 개별 SHA/크기를 재검증했다. 약8.28GB의 해당 폴더 전체를 복원하지 않았고 vendor·실행 바이너리·원본 모델/텍스처·큰 로그·영상은 제외했다.

기존 source-fence 관찰 JSON 약41.3MB는 검증된 ZIP 안에서 읽고 SHA를 대조한 뒤 집계만 저장했다. 원문 전체를 추가 추출/공개하지 않았다. 복구한 Python65파일 AST 파싱과 기존 오프라인 계약 시험16개를 실행해 모두 통과했다. Java/Enforce 재컴파일·PBO 빌드·게임·후킹·새 영상은 미실행이다.

## 3. 측정 결과

아래 게임 수치는 2026-10-11~12의 기존 기록이며 이번 신규 실험 수치가 아니다.

|단계|확인된 내용|판정 경계|
|---|---|---|
|초기 좌표·ACK 연결|M2 완결 왕복3,543건 대조. M3 실제 응답20개 p95약412.284ms, stale 요청2021ms timeout 기록|파일 관측 왕복이며 입력→화면 지연이나 NPC 물리 비용 아님|
|원본 무리 표현|64 source/64 host,122표본,57명0.1 이상 이동,60개 개별 포즈,ID 대응. 위치 readback 최대약0.000546m|원본 남31/여33 몸체·포즈 표시. 완전한 충돌/전투/의복 이식 아님|
|원본 본 갱신 조건|같은64 ID 두25초 구간, 포즈 변화3→64, 이동중 포즈불변 표본1369→84. 추가 Update호출0|한 번의 해제/재활성화 대조. CPU 절감률/64명 완전 시각 품질 아님|
|원본 세계 AI 반응|보관 raw source-fence 시험194기록,error null,최대9명 플레이어 target,8명 울타리 반대편 이동,넘기 상태153개체표본|원본 PZ 울타리에서 명시적 자극을 준 시험. DayZ 울타리 입력/동일 월드 증거 아님|
|후속 단일 원본 포즈 표시|35본×32 대응프레임 최대이동오차 약0.087mm. 별도 정적 기준 약0.0114mm|동일 포즈/기록의 표현 정합. '64프레임'은64개체 아님|
|보간 표시|원본 관절계층 보간·8프레임·클라이언트 수신시각 기준500ms 지연. 20.128초2480갱신,보간96.8145% 기록|123.21갱신/초는 코드실행 빈도이지 화면 FPS/원본 pose rate 아님|

후속 원본 코호트는 전멸했고 새 단일 개체·기록 재생·진단 복제본으로 표현을 검증했다. 처음의64명 성공을 최종1개체·정적/동적2복제본 화면과 혼합하지 않는다. 과거 영상 분석 보고서는 있으나 이번에는 영상 자체를 복원/재판정하지 않았다. 전체 CPU/Present p50/p95/p99와 현재 DG 연결 지연은 미측정이다.

## 4. 원인 분석

### 앞선 설명의 정정

앞서 '위치 전달만 있었고 원본 표현은 없었다'고 설명한 것은 초기 M2/M3만 반영한 불완전한 설명이다. 복구된 후속 코드와 검증 기록에서는 실제 PZ 원본 몸체·35본 포즈·64개체 표현, 제한된 관찰자 프록시까지 수행했다. 기존 11-04 결합 보고서를 소급 수정하지 않고 이 정정과 새 근거를 연결한다. 다만 이것이 원본 게임 전체 이식이나 공유 물리 완성을 뜻하지 않는다.

### 실제 실행 구조

```text
PZ 엔진: 실제 IsoZombie의 AI/경로/애니메이션 실행
 → Java agent: Lua 노출 보강 + 원본 AnimationPlayer 행렬/골격 읽기
 → Lua: 고정 코호트 ID·위치·상태·35본 행렬,250ms 목표 파일 출력
 → Python: 세션/기한/ID/골격·몸체 검증 + 좌표/스킨 변환
 → DayZ 서버: 제한된 Building 기반 시각 개체,위치/210채널 적용 + ACK/RPC
 → DayZ 클라이언트: 수신 버퍼·부모별 로컬 포즈 보간→계층 합성→표시

제한된 역방향:
DayZ 관찰자 좌표 → Python → PZ 플레이어 프록시 위치/가시성
 → PZ 원본 감각·경로 반응. DayZ 세계 기하 입력은 없음.
```

Java는 reflection으로 getAnimationPlayer/getModelTransformAt와 스키닝 메타데이터를 읽고 Lua에 함수를 등록한다. Lua 직접 포즈 접근은 non-exposed userdata로 실패해 Java 시작 agent를 추가했다. 후속 ClassFileTransformer는 원본 updateAnimPlayer의 updateBones=false 지점을 임대된 개체에 한해 바꾼다. 설치 파일을 바꾸지 않았지만 JVM 실행 코드 조건을 바꾸므로 읽기 전용 브리지가 아니다. 기존 Update를 한 번 더 호출하지 않는다. DG에는 같은 Java/Lua 접근을 적용할 수 없다.

DayZ 수신은 추상적인 가능성만이 아니라 이미 구현된 file mailbox→서버 객체→RPC→클라이언트 표시 경로다. 다만 AnimalBase 네이티브 애니메이션이 아니라 Building+35본/210 model animation채널 표현이며 현재8파트 NPC 파이프라인과 다르다. Days Gone248본과 실제 weighted본/바인드·파트 구조에 그대로 끼울 수 없다. 현재 NPC에 보간을 이식하라는 권고도 아니다.

### 재사용 판정

|부분|재사용 수준|수정·제약|
|---|---|---|
|session/sequence/기한/ACK/부분파일·유한값 검증|높음: 자체 코드·기존16계약시험 재사용 후보|M3는단일 marker용,후속horde wire는별도. 하나의 완성 범용규격으로 취급 금지|
|epoch+actor ID,고정 코호트·퇴역·몸체 불일치 격리|설계·일부 코드 재사용|PZ 객체참조와 DG 포인터/세대 ID는 다름. DG generation 확인 필요|
|파일 mailbox·원자적 교체 재시도·소유 프로세스 검증|재사용 후보|경로 하드코딩/당시session/설정 제거 필요. 일부Lua/observer 쓰기는 원자적이지 않음.250ms·ACK대기는 저지연보장 아님|
|DayZ 서버 수신·생성·삭제·ACK·RPC·클라이언트 재생|직접 활용 가능한 참조 구현 확보|35본/210채널·남녀2몸체·고정장면 한정. DG 전용 모델/스키마/본 매핑 필요|
|정적 CPU 포즈와 실제 엔진 본 비교|검증 도구 원리 재사용|PZ 행렬 규약/피벗·계층을 DG 규약으로 교체. 현재 동작하는8파트 코드와 격리|
|순서 보존 버퍼·부모 로컬 회전 Slerp|구현/실패 이력 재사용|500ms지연을 사용. 전투 실시간성과 비용 미수용. 기존native시계 수정 대체 아님|
|관찰자 프록시|권한·기한·좌표응답 구조 재사용|PZ Lua teleport/visible 호출을 DG 해당스레드의 검증된 입력으로 교체해야 함|
|공격/피격 메시지|nonce/sequence/중복방지 계약·PZ native Hit 후보 코드 존재|실제 DayZ 총기→PZ피격→양쪽사망 전체 연결 성공 증거 없음. 양방향 전투 완료로 보고 금지|

### Days Gone에서 새로 확보해야 하는 부분

1. source 접근부: 일관된 NPC 위치·회전·골격/포즈·행동·생명주기 export. 기존10Hz 외부 읽기는 완결 pose snapshot이 아니다. 실제 최종 포즈 버퍼와 업데이트 단계·객체세대 식별이 필요하다.
2. 입력부: DG 플레이어 프록시를 DayZ 관찰자와 대응시킬 수 있는지,또는호드 목표를 제어할 수 있는지. 기존노클립은플레이어위치 호출 근거일뿐 지속AI목표제어/호드명령계약 아님. 기존 함수 호출 스레드 제약 준수 필요.
3. 표현부:248본 원본/224weighted 메타데이터와 현재8파트 구성에 맞는 상태·포즈 전송/표시 경로. PZ35본 모델을 단순 확대하면 비용 증가 가능성이 있으나 절감/증가율은 미측정이다.
4. 월드부: DayZ 지형·동적 장애물·길찾기 연결을 DG에 제공하거나 질의를 중계할 경로. PZ때도 해결하지 못한 핵심이며 좌표변환/원본플레이어프록시로 해결되지 않는다.
5. 상호작용:충돌·지지·피해·사망의 권한을 하나씩 고정하고 중복적용·지연·원본개체언로드를 처리. 시각 객체의 위치 설정을 실제 안전한 물리 이동으로 취급하지 않는다.
6. 비용:두 게임 동시실행·export·변환·RPC·표시 총비용. 원본20생존지붕과커스텀64평지의 과거프레임시간만으로 결합 성능을 예측하지 않는다.

### 필요한 도구

기존Python·REA/Ghidra·Capstone·Frida·DayZ Tools/Blender가 우선이다. 새 구매·전면설치 필요 근거는 없다. PZ Java agent/JDK25는 PZ 복구 재빌드용이며 DG 접근 문제를 해결하지 않는다. DG의 검증된 네이티브 export/input 어댑터는 새로 필요하다. 고빈도 pipe/shared memory 선택은 수신 API와 저속 계약 검증 뒤 판단한다. 공식 모드 배포와 진단용 후킹은 별개이며 온라인/BattlEye 환경의 허용·작동은 미확인이다.

## 5. 검증 수준

복구무결성:221/221 SHA일치,선정ZIP3/3전체SHA일치. Python문법65파일·오프라인계약16/16:현재통과. 기존PZ 시각·제한AI반응:과거기록/소스와선정raw집계대조. 영상재검토·현재게임실행·Java/Enforce컴파일·서명·DaysGone브리지·현재8/64회귀·새성능:미실행. 원본호드와DayZ 전체공유월드·전투는미완료.20ms목표달성아님.

## 6. 변경 사항

기존PassthroughLab·현재Source/PBO·게임설치·서명키·원시기록은 그대로다. 새research복구본과복구/검토도구·체크포인트만추가했다. 복구한launchers는실행하지않는다. 과거PID/기한/상대경로·설치버전이남아있어현재실행준비완료로표시하지않는다. 과거goalComplete/boundedGoalAchieved는단일표현작업의조건부완료이며전체프로젝트완료로승격하지않는다. 새PBO없으므로새서명검증해당없음.

## 7. 원시 증거

복구검증:recovery-verification.json,offline-validation.json,archived-evidence-projection.json. 설치구독Recovery의고정index224f4b64…a9f2,manifestbb05619b…481,필요shard0001/0002/0005의SHA검증후선정멤버CRC/SHA/크기대조. 파일마다원래상대경로·SHA·크기를보존했다.

핵심코드:OriginalPoseAgent.java/OriginalPoseBridge.java,PTOriginalHorde.lua,relay_original_horde.py,couple_original_observer.py,PZOriginalHorde.c/PZOriginalVisual.c/PZHostClient.c,bridge_contract.py,original_combat_protocol.py. 핵심기록:M2/M3,original-horde-import-verification.json,original-bone-promotion-verification.json,original-renderer-completion-audit.json,original-renderer-goal-progress.json,original-live-playback-observation.json. 공개evidence는자체코드선정발췌·행범위·SHA와민감정보제거집계다. 원본게임자산/영상/전체raw/로컬절대경로/PID/키는공개하지않는다. 원시영상미확보판정은이번재검토의경계이며보관에없다는뜻이아니다.

## 8. 잔여 문제

다음단일산출물은 '기존DayZ수신기를재사용할수있는DG source/input계약표'다. export할포즈인지상태/클립인지선택하고,그데이터가만들어지는원본실행단계와DayZ소비경로를연결한다. 아직브리지구현/재설계를시작하지않는다.

연결최소시험을제안한다면공통고정장면1개체의실제원본상태전달→표현→프록시목표반응만먼저판정한다. 기록재생·live전달·공유월드·전투·64성능을각각분리한다. PZ의500ms지연버퍼와35본표현을그대로현NPC에적용하지않고실행경로를분리한다. DG입력/포즈export가특정되지않으면전면디컴파일이나PZ실험반복으로범위를확대하지않는다.
