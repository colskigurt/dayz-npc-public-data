# 격자 셀 풀 — 실제Enforce 메서드동등성

## 1. 작업 목표
기본OFF 격자 셀 풀 후보가 실제 Enforce에서 컴파일되고 동일 입력의 접촉 계산을 유지하는지 확인한다. 아직 전체CPU/물리질의 최적화 성공을 판단하는 단계가 아니다.
## 2. 실제 수행
기존 서버하네스와 서명PBO를 사용했다. 최초 실행 전 manifest가 LF문자열 해시와 실제CRLF파일 해시 차이로 거부되어 시작하지 않았고 실제파일 해시로 수정했다. method1은 미션 산술식의 공백 없는 뺄셈에서 컴파일 실패했다. method2는 문법 수정 후 시험 진입표식/TSV 없음,method3은 미션생성표식은 있으나 OnUpdate 기반 시험 진입증거 없음이었다. 세 실행을 제외하고 PID/생성시각/명령행 확인 후 해당 빈 시험서버만 강제 종료했다. 원시로그와 cleanup을 보존했다. runner의 forcedStop=false는 외부정리 전 호스트 값으로 실제 강제정리 receipt가 우선한다.
method4는 같은 후보PBO로 생성자 예약 호출에서180개 입력세트를 비교했다. 실제 NPC나 클라이언트는 생성하지 않았다. 원본OFF와 후보ON에 같은64개 메타데이터 body와 위치/속도/DEAD/FOLLOW/LAND/zero dt/큰dt를 제공했다. 변수상태와 접촉계수,힘·속도·변위제안·입구우선순위의 exact비교를 매 입력세트 수행했다. OnUpdate 미진입의 원인은 특정하지 못했으며 예약호출 성공을 그 원인 증명으로 사용하지 않는다.
## 3. 측정 결과
성공 method4 입력180세트,합산 PairCandidates221991,ContactPairs57296,불일치0. 풀생성64/재사용3728. Enforce World/Mission 로드 및 결과TSV180행 확인. 이것은 한 예약호출 안의180회 메서드 비교로,180게임프레임 회귀나 실행중 실제64NPC시험이 아니다. p50/p95/p99·할당량·GC·실제화면 미측정.
## 4. 원인 분석
모델뿐 아니라 실제 Enforce의 접촉힘 누적과 속도적분이 선택조건에서 일치했다. 셀 재사용은 실제 동작했으나 native Bullet 질의를 줄이는 변경은 아니다. Cells 맵과 키 문자열은 기존대로 매번 처리한다. EntryPriority는 OpenTraversal=true라 조기반환했고 플레이어집합은 비어 있어 지지/플레이어검사를 시험하지 않았다. ConsumeContact/Displace/SolveContacts/피격/사망엔티티/네트워크/렌더 미실행. CPU 이득은 증명되지 않았다.
## 5. 검증 수준
실제Enforce 메서드동등성 단일실행 통과.8/64플레이 기능·시각·반복성능 미실행. 이전 오프라인700세트 및 서명빌드 결과와 구분한다. server RequestExit0 뒤 actual exitCode−1,normalProcessExit=false. 프로세스는 모두 소멸했지만 정상종료 통과로 보고하지 않는다. 제외실행3개를 성공 반복으로 세지 않는다.
## 6. 변경 사항
격리 method1~4 미션/프로필·분석만 추가. PBO는 기존 후보 SHA 3CAA0D50D4CF64498FD8C98834F324F89B2CEDBE66AC4382DBCC3D73B94C9AC7 그대로이며 기존 빌드 서명검증 성공. 새 게임코드/PBO/키/운영서버 변경 없음. 기본OFF 유지.
## 7. 원시 증거
ANIM-grid-pool-method1~4 미션/manifest/서버RPT·script·실행상태/cleanup,method4 grid-method.tsv180행,method-results.json과analyze-method.py. source-equivalence-proof와이전build/signature를연결한다. 원시증거 해시를 검증했다.
## 8. 잔여 문제
다음은 기존8개체 하네스에 후보만 추가한 제한적 기능시험이다.정상 밀침·낙하·착지·FOLLOW·재등반을 유지하고 별도 기능개발로 확대하지 않는다. 기능확인 뒤64 비용screen에가되 전체CPU/p95이득이없으면 재사용계수만으로채택하지않는다. 이전 실패후보를누적적용하지않고 현재5파트기준을사용한다. 애니메이션품질은 이번시험대상이아니며 기존사용자평가와미검증범위를보존한다. 전체20ms는목표다.
