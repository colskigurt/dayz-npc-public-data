# 컨트롤러 연결 확인:물리 리더 서버3회 완료,갱신200ms·시각개선미판정

작성: 2026-11-01T15:10:53.994740+09:00

## 1. 작업 목표

서버 평가 경로에서null이던 context+8의 연결 후보를 좁히고, 올바른 물리 역할의 기존 자산에서 동일 경로를 안전하게 관찰한다. 초기 생성·갱신 주기·시각 품질을 구분하고 기존 오프라인 NPC 파이프라인은 변경하지 않는다.

## 2. 실제 수행

고정 서버 PE의0xA40000~A60000 영역540개 unwind 구간에서+8 포인터 쓰기 후보를 분류했다. offset 일치만으로 소유자를 확정하지 않고,0xA497F0의 생성 반환값 기록과0xA46A50의 생성자를 따라 BulletCharacterCtrlSimple/BulletCharacterCtrl RTTI를 확인했다. 후보의 직접E8 caller는 못 찾았고 간접 경로는 미완전이다.

기존1파트 fixture에 ECE_CREATEPHYSICS만 추가한 최소 대조는 생성 단계에서 다른 위치로 충돌했다. 약701KB의 보존된 소형 dump에서 exception context 경계와 RIP 일치를 확인해 RAX=0, 인덱스의 하위 signed8 값=-1을 얻었다. 전체 중첩 memory stream 완전성까지 인증한 것은 아니다.

같은 기존PBO 안의 DGSubsetIntegratedCrowdLeader 클래스로만 바꾼 비교는 완료됐다. 같은 fixture를 세 번 완료했으며 처음 두 실행은 관찰기 없이, 마지막은10초 관찰 훅으로 수행했다. 관찰기 준비 중 cached Frida 경로를 빠뜨린 import 실패가 있었고 그 단계에서는 attach하지 않았다. 기존 캐시 경로를 확인해 고쳤고 설치는 하지 않았다. 새로 살아 있는 소유 프로세스·바이너리·CLI 범위를 확인한 마지막 실행만 실제 attach했다.

마지막 관찰은 수치/모드/포인터를 쓰지 않았으며 훅을10초 후 끄고 서버 종료까지 유지했다. 기록 파일을 닫은 DONE 뒤 소유 서버를 종료했다. 이는 정상 서버 종료라고 기록하지 않는다. 클라이언트·새PBO·영상·운영 서버 변경은 없다.

## 3. 측정 결과

서버 시작4회: 렌더 파트+물리 플래그1회 실패, 물리 리더1파트3회 완료다. 리더별 포즈 행은1251/1251/1252개, 채택 mission 시간은25.000/25.002/25.009초다. 스크립트 command active 비율은 모두1.0, crash0이다. 이는8/64 NPC 기능 회귀가 아니다.

마지막 관찰은72표본/1엔티티,오류0,수치변경0이며 context+8이 모두BulletCharacterCtrlSimple의 정적RTTI vtable과 일치했다. 따라서 이 성공 fixture에서는+8이 캐릭터 물리 컨트롤러를 가리킴을 직접 관찰했다. 이전 실패 순간의 모든 포인터 상태까지 복원한 것은 아니다.

물리 플래그만 추가한 실패의 RVA는0xAA8AE9,명령 movss xmm0,[rax+0x54],RAX=0이다. 이전 생성 후 평가 충돌0xA52EE4와 다르며 단순 재현 성공으로 묶지 않는다. native signed 인덱스=-1은 확인했으나 그 인덱스를 특정 LOD 이름에 직접 연결하지는 못했다.

현재 source MLOD는 렌더 파트에Geometry1e13이 없고 리더에는 있다. 실제 packed 모델은ODOL이라 내부Geometry를 직접 decode하지 않았으며 현재source와compiled의 완전 동등성도 미인증이다. 모델 역할 변경의 효과와 소스 차이는 근거지만 단일 root cause 증명은 아니다.

세 성공 실행의 관측 command 시간 증분 p50/p95/p99는 약200/200/200ms다. NativeSeconds/mission초 비율은1.000000/0.999920/0.999640이지만 NativeSeconds는스크립트 누적기이며native graph clock이 아니다. CPU 머리 포즈 반복 쌍은1125/1250,1125/1250,1126/1251이었다. 이를 화면 frame drop 수치로 읽지 않는다.

탐색적 머리 좌표 스펙트럼 최강 피크는 약0.48Hz(2.083~2.085초),주파수 격자약0.04Hz다. 이는 한 관절의진동이며 고조파·샘플링 영향을 포함해 정확한clip clock/시각 품질을 증명하지 않는다. mission 입력dt p50/p95/p99=15/31/31ms도 CPU/Present frame-time이 아니다.

새64 기능/CPU·GPU/Present·할당·GC·시각 품질은 미측정이다.20ms p95는 목표로 유지한다.

## 4. 원인 분석

확인: 적어도 올바른 물리 역할의 기존 리더 fixture에서는mode1 평가와 실제 컨트롤러 연결이 동작한다. 앞선 렌더 파트 실패를 모든 서버 애니메이션의 불가능으로 확대하면 안 된다. 반대로 렌더 파트에 물리 플래그만 추가하면 이번 조건에서 충분하지 않았다.

확인: 기능상 초기 충돌을 피한 성공 fixture에도 약200ms의 script command 갱신과 CPU 포즈 반복이 있다. 서버에서 애니메이션이 진행된다는 사실을부드러움 개선으로 판정할 수 없다. 관찰 훅이 있는 실행은 비용 비교에서 제외한다.

가설: 자동 simulation LOD 또는 서버의 다른 scheduling 조건이200ms 갱신에 영향을 줄 수 있다. 아직 LOD 필드를 직접 읽거나 통제하지 않아원인으로 확정하지 않는다. 생성 실패의 인덱스·Geometry/컨트롤러 초기화 관계도 추가 native 생산 경로 확인이 필요하다.

## 5. 검증 수준

컨트롤러 종류의72관찰은 진단 성공이다. 물리 리더1파트의 bounded fixture 완료는 관찰기 없는2회와 관찰기 있는1회로 반복 확인했다. 전체NPC·8/64회귀·시각 품질·성능·최종 수용은 미완료다. native memory 조작이나 mode 강제 쓰기로 얻은 성공이 아니다.

## 6. 변경 사항

격리 research에 작은 fixture/관찰기/분석 JSON과 profile만 추가했다. 기존 PBO·게임 코드·ANM·운영 서버·9350 후보는 보존했다. 기존3PBO 해시·서명은 매 실행 전 검증 성공했다. 새 PBO가 없어 새 서명 대상은 없다. 원시 dump는 로컬 보존하며 공개하지 않는다.

## 7. 원시 증거

animation-context-attachment-20261101-1449. role-results/controller-observer/physics-failure-detailed/model-role-audit/head-spectrum과각profile·launch·source·명령구간 해시를 연결한다. 서명 도구/서버/관찰기는 모두 종료됐다. 공개는 집계·선정 포즈/함수·vtable RVA·register zero mask이며 PID·ASLR base·raw command line·원시 dump·모델 자산은 제외한다.

## 8. 잔여 문제

다음 최소 실험은 같은 물리 리더에서자동LOD와공개 UpdateSimulationPrecision(0) 한 요인을비교하는 것이다. command 갱신·CPU 포즈·native clock을구분해관찰하고,200ms가줄지않으면다른scheduler생산경로로돌아간다. 실제 clock proxy가없으면 CPU/command만확인됐다고명시한다. 필요한자원은기존loopback서버와현재3PBO이며클라이언트/영상은이실험에불필요하다.

충돌·서명불일치·엔티티수범위이탈·관찰오류가나오면해당후보를중단하고증거를보존한다. 성공해도시각품질과64성능승인은별도다. 모든렌더파트에물리를추가하거나현행파이프라인을서버형으로재설계하지않는다. 현재scope는역할/초기화/시간진단이며정상밀침·낙하·재등반의기존계약은보존한다.

선정 공개 본문. 로컬 보고서 SHA-256: `b6210b920facc3ccd8a46e633d9ba7b8d90189f2ba45e21df720bf2c4d8114c9`. 원시 전체가 아니다.
