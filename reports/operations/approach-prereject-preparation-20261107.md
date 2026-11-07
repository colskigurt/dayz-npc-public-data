# 접근 후보의 조기 점유 탈락 — 계산량 절감 후보 준비

## 1. 작업 목표

Days Gone 수치 분류보다 DayZ 실제 계산량 감소를 우선한다. 기존 단일solve·무조건 캐시·SupportSweepFast·레인 확대·Scratch 결과를 재사용하고 안전하게 제거 가능한 목적지 후보 검사를 선별한다. 골은 재개 후 active 확인.

## 2. 실제 수행

기존 Plan은 center+4개 offset Floor, SegmentClear, CoreClear를 수행한 뒤 Claimed 목적지를 탈락시킨다. Floor는 높이를 보정하므로 사전 raw 좌표에서 Claimed를 호출하는 안은 피했다. center Floor/area 통과 직후 보정된 support로 Claimed를 검사하는 기본OFF 격리 소스 사본을 만들었다. 점유 목적지는 주변4점 검사 전에 탈락하며 나머지는 기존 검사를 통과해야 한다. retained lease·sampled navmesh·Drive·Displace·낙하·착지는 수정하지 않았다.

기존 B8FB Plan wrapper가 선행 경로 재검증 후 super.Plan을 부르므로, 새 modded Plan 전체 복사로 해당 wrapper를 우회하지 않는다. 준비 사본은 베이스 소스 변경 후보이며 기존 PBO나 실제 소스는 변경하지 않았다. 실제 패키징 전 베이스와 wrapper의 로드 결합을 별도 확인해야 한다.

## 3. 측정 결과

고정된 predicate 결과를 전수 조합한 오프라인320조건에서 최종 수용/거부 불일치0. 모형 Ray 호출544→432, Claimed 호출4→80. 이는 현실 분포가 아닌 인위적 진리표이며 성능 수치가 아니다. 특히 기존에는 발판/벽에서 탈락해 Claimed까지 오지 않던 후보에도 조기 점유검사가 추가될 수 있어 전체 CPU가 증가할 수 있다. 신규 게임0, PBO빌드0, Enforce컴파일0, 기능/시각/CPU/Present 신규측정 없음.

## 4. 원인 분석

확인: 선택될 수 없는 점유 후보에도 지지 주변점/벽 질의를 먼저 실행하는 구조. 후보는 결과 캐시나 물리 판정 삭제 대신 실패 확정 시점의 작업을 줄이는 순서 변경이다. 기존42쌍 입력 불일치와 단일solve 계약 기각을 번복하지 않는다.

가설: 점유 탈락이 충분히 자주 발생하고 Claimed 순회비용보다 생략되는 물리 질의비용이 클 때 이득. 반대 조건에서는 손해다. 아직 빈도·self-time 근거가 없어 주력 채택 후보가 아닌 선별 후보로 유지한다. 동기 Plan 실행 동안 peer 상태/geometry-query 부작용이 결과를 바꾸지 않는다는 전제가 실제 엔진에서 검증되지 않았다.

## 5. 검증 수준

격리 소스 준비 및 오프라인 고정-predicate 계약 진단 통과. 실제 물리/엔진 동일성·컴파일·8/64회귀·반복성능·최종수용은 미실행. 준비를 최적화 성공으로 보고하지 않는다.

## 6. 변경 사항

approach-prereject1 아래 후보 DGBNativePursuit.c·diff·검증 스크립트·JSON만 생성. 기본OFF profile flag. 원본 source SHA ac51917d47ce5cc75a044084e027b07fd2a812dd8362014068653aff60ff2062; 후보 dbfe4498a4685f61cb0187130705d126dcc98c9aa1d8aa0e7011188af47b1957. 새PBO·배포/서버/실행기 변경 없음, 서명대상 아직 없음. 빌드시 서명·검증 필수.

## 7. 원시 증거

실행 ID approach-prereject1. source-proof.json, candidate.patch, offline-contract.json, verify-offline.py. 기존 work-reduction-lane-feasibility·lane-spacing-eight-pair1·collector-exit-contract·direct-cost-ownership-review와 COMBINED_PROPOSAL_FEASIBILITY를 재사용했다. 저장 전 해시 검증, 기존 증거와 원본 보존.

## 8. 잔여 문제

다음 단일 gate: 기존 메서드 하네스에서 baseline/candidate 입력·출력·실제 Floor/Claimed 수·경과비용을 OFF/ON 교대로 비교한다. 사전조건은 실제 베이스 PBO/script 일치와 기존 Plan wrapper 보존, 서명검증이다. 합성 계약 불일치·ON 비용 증가·실제 early-claim hit 부재이면64시험으로 확대하지 않는다. 유망할 때만8동작→64 동일조건 반복 비교. Days Gone 실행 중 전체 게임 성능시험을 겹쳐 하지 않는다. 기존64 수집 크래시·timing-only 미수용 결과는 별도로 보존하며 기능 복구 작업으로 확대하지 않는다. 전체 골 미완료, active 유지.
