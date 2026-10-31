# 실제 root evaluator 시간 문맥:50ms 입력과80ms 호출 간격

작성: 2026-10-31T09:54:39.852231+09:00

## 1. 작업 목표

실제 스켈레톤 evaluator의 가상 호출 진입에서 받은 시간 문맥을 포착해 기존50ms 시간 소비·초과분 reset 증거를 한 단계 하류와 연결한다. 일반 IEntity.Update를 스켈레톤 시계 setter로 오인하지 않도록 설치 API·바인딩·호출 순서도 대조한다. 기존 구현·낙하/복구·운영 서버를 보존한다.

## 2. 실제 수행

기존 B8FB 고정 run clip2·논리1개체/네이티브8파트·AI/루트 이동OFF·120초 클라이언트를 실행했다. 전경5초 이상을 확인한 뒤 기존 Frida entry-only 관찰기를8초 연결했다. NativeAnimal receiver를 추측해 일반 Animate를 호출한 것이 아니다. 선택73본 renderer의 실제 호출 반환주소0xb1d1f1·실시간 vtable0x11c7cc8·첫 가상 함수0x4906b0·stack timeContext 위치를 검사했다. 관찰 뒤 훅을 비활성화하고 클라이언트의 정상 종료까지 유지해 hot unload하지 않았다. 인자·결과·누적시간을 덮어쓰지 않았다. 도구는 기존 Python Frida이며 REA MCP가 디컴파일한 결과로 표현하지 않는다.

정적 보조 분석은 설치 EXE 원본 바이트와 scripts.pbo를 사용했다. IEntity 부근 등록에서 GetVObject/Update/Animate/AnimateEx의 서로 다른 두 등록 helper 형식과 LEA rax→mov r8 형식을 확인했다. Update handler0x2cfe80는 entity+0x78의 가상 slot0x338을 선택적으로 호출하고0x4356b0로 tail jump한다. 같은0x4356b0가 선택 스켈레톤 경로의 평가/보간 뒤에도 호출된다. helper의 모든 분기/virtual effect는 해석하지 않았다.

실제 scheduler의 선택17개 명령 바이트도 재확인했다. mode0 경로는 입력과entity+0x66c를 더해 interval+0x250과 비교하고,임계값 도달 시 평가 wrapper 호출 뒤+0x66c에0을 쓴다. 상위 가상 슬롯0x880이0xb6d390을 가리키는 것도 확인했다. 새 Ghidra startup은 실행하지 않았다.

## 3. 측정 결과

- 새 실행 runtime-20261031-094846,정상 종료.8초 관찰1회,선택 root 가상 진입 **100회**.
- 관측 전체 진입1040회 중 다른 skeleton940회 제외. 선택 renderer1개,evaluator 객체8개. RTTI는 IAnimEvaluator이며8개 객체를8개 논리 NPC로 해석하지 않는다.
- graph descriptor dt p50/p95 **50.000000745/50.000000745ms**. 실제 timeContext dt p50/p95 **50.000190735/50.000190735ms**.
-100회 모두 contextDt=float32(float32(oldClock+graphDt)-oldClock)와 정확히 일치,최대 잔차0ms. oldClock와owner clock 차이도0ms.
- 호출 간격 p50/p95/p99 **80/82/83.02ms**. context oldClock 증가4950.018883ms,host Date.now span7882ms,진행 비율 **0.628016**.
- observer/script 오류·drop **0**,실행 파일 해시 불변. 관찰 훅 사용이며 read-only VM 표본화만 한 것은 아니다.
-15~55초 전경 mission frame p50/p95/p99 **9.0/35.0/37.0ms**,전경 비율1.0,콜백 12.558830Hz,script nativeSeconds/missionSeconds 0.627936.

이번에는 동영상 녹화·GPU Present·최종 clip sample time·8/64 기능·64비용 대조를 측정하지 않았다. HUD visualCapture 플래그는 실제 영상 저장을 의미하지 않는다. 위frame 수치를 최적화 개선 또는64개체20ms목표 달성으로 사용하지 않는다.

## 4. 원인 분석

확인: root evaluator에 도달한 시간 문맥도 약50ms이며 약80ms 간격 호출과 함께 문맥 시계 진행이host의62.8%로 관측됐다. 이전 descriptor/owner clock 표본을 넘어 실제 가상 메서드 진입 인자까지 연결했다. 단순한 CPU 로그 이름만으로 해당 인자라고 가정한 것이 아니다.

범위: 포착한 것은 **받은 시간 문맥**이다. 이 함수 이후 state/clip node가 어떻게 시간을 소비하거나 보정하는지,최종 clip phase와GPU 표시까지 같은 손실이 그대로 이어지는지는 미확인이다. 과거 CPU/영상 연결 증거가 있더라도 이번에 실제 영상 동기화를 수행했다고 주장하지 않는다. float32 차이의미세오차를 수십ms 호출 간격 차이의 원인으로 단정하지 않는다.

IEntity.Update의 설치 주석은 위치·상태 변경을trace전에 commit하는 계약이다. 확인된 tail helper는 스켈레톤 처리 뒤와 공유하지만 그 사실만으로 Update가 다른 경로에서 평가를 절대로 하지 않는다고 증명하지는 못한다. 독립16.67ms 평가/잔여시간 setter는 확보되지 않았다. 일반 Update/Animate 호출이나 새시간 변수를 무작정 적용할 근거로 사용하지 않는다.

## 5. 검증 수준

- 선택 실제 root 가상 진입/수신 시간 문맥: 진단 성공,1회100표본.
- 기존 scheduler/reset 및 public Update의 선택 바인딩/순서: 원본 바이트 정적 대조. 전체 CFG/모든 virtual target 검증 아님.
- 최종 clip time·실제 표시·지원 제어·수정 효과·반복 회귀·64비용·20ms p95·전체 골: 미완료.

## 6. 변경 사항

기존 빌드 B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185 유지. 실제8/64 기능시험의 CBFB observer와 다른 격리 clip 환경이다. 새 PBO·서명·배포는 없으며 실행 전3개 의존 서명 검증은 통과했다. 관찰기·분석·계획과 정적 보조 파일만 추가했다. 인자/descriptor/numeric override는OFF다. 운영 working D61D·운영 서버·원시 기록·개인키는 보존했다.

## 7. 원시 증거

plan.json·attach-readiness.json·scheduler-link.json·evaluator-rtti.json·results.json,profile의 native-call-probe/calls.jsonl·status.json·configuration.json·instrumentation.js·root-time-context-analysis.json·launch.json·controlled-analysis.json·visible-frame-clock.tsv를 연결했다. source plan의3개 도구 SHA와실제 사용한5개 raw해시를대조했다. animation-update-contract-20261031-094500의 registry-region.json/results.json과 원본 설치API 문맥·바인딩도 포함한다. .pdata가 없는leaf는 범위를추측해fullfunction으로표시하지않았다. 공개본은 선정 이벤트6개와 요약/코드근거이며 실제주소·스레드/프로세스정보·원시전체·게임자산은 제외한다.

## 8. 잔여 문제

다음은 evaluator 이후 실제 state/clip sample-time 소비 지점을 좁혀 문맥 시계 손실이 최종 포즈 시간에 어떻게 반영되는지 확인하는 것이다. 그 지점이 확인되고 지원 제어 후보의 단위·reset·보간 세대 계약이 확보되기 전 새로운 보간·프레임 생략·float 시간 주입을 적용하지 않는다. 평균만 정상이고 세대 경계 jump/held frame이 악화된 과거 모델은 다시 사용하지 않는다.

public Update의 위치commit 계약은 즉시fixture검사의 환경순서 진단에 참고할 수 있으나 이번에는 호출 시험을 하지 않았다. 필요하면 별도 격리 시험으로 검증한다. 기능 실패·동적 복구 재현과64비용 대조는 각각 별도 조건으로 이어가며 이번1개체hook비용을 섞지 않는다.

선정 공개 본문. 로컬 보고서 SHA-256: `376445c725305a8673cf743a8c2c90cf231d0f59b1065c9af0d4e9dea90e61d3`. 원시 전체가 아니다.
