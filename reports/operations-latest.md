# FOLLOW 내부 계측 패키지:8개체 실제 기록과 기능 검사

작성: 2026-10-31T08:24:41.754506+09:00

## 1. 작업 목표

현재 B93D 64개체 기준 측정에서 FOLLOW가 Traversal 경과시간의 약90%였지만 내부 항목은 합쳐져 있었다. 안정된 원본을 보존하고 별도 기본OFF 계측 패키지로 FOLLOW 내부 함수 경계를 구분할 수 있는지 검증한다. 새 최적화·애니메이션 수정은 하지 않는다.

## 2. 실제 수행

B93D의 기존14개 스크립트와 config를 바이트 동일하게 복사하고 타이머 파일 DGSubsetZZZZZZZFollowCost.c 하나만 추가했다. Follow/ResolveApproachArea/PlanConnection/SearchApproach/FollowSurfaceYield/SurfaceWalk/Walk/DriveGround를 기존 super 인자와 반환값을 유지하는 방식으로 감쌌다. 모듈별 중첩 경과시간과 호출 횟수를 기록하며 마지막 개체의 프레임 낙하 호출 후 표본을 저장한다. follow-cost-split.flag가 없으면 원래 호출로 통과한다. OFF도 래퍼 호출 자체는 남으므로 비용이 원본과 완전히 같다고 가정하지 않는다.

초기 v1은 Count()-1 식의 토큰 구문에서 World 컴파일이 실패했다. v2에서 공백만 고친 뒤 World는 통과했으나 시험용 mission helper의 예약어 out 사용으로 두 번째 컴파일이 실패했다. 두 실패 빌드/소스/로그를 보존했다. 새 mission fixture에서 해당 지역 변수만 policyFile로 바꾸고 OFF/ON/실제씬 세 엔진 검사를 통과했다. v2 패키지 E2DC091FB183DB0A656AFA2438DF32FAE0195ED54C8AC75D9AFDED496F64EF5B를 자동 서명·검증했다.

실제 실행 runtime-20261031-081814, 300초, 8논리 개체·64개 렌더/애니메이션 파트, 고정 카메라, 기존 고지대·지면 시나리오, 기본 기능 후보 플래그ON과 기존 DriveAttribution 및 추가 FollowCostSplit ON. 이 패키지의 새64개체 실행과 비용 OFF/ON 반복은 아직 없다.

## 3. 측정 결과

고지대 FOLLOW 8/8, 최종 도착 8/8, 정상 낙하 후 착지·추적 재개 9회, 등반 종료 실패 0, 미해결 종료 0. 단일 기능 게이트 True. p50/p95/p99는 **20.0/34.0/35.0ms**, 전경 비율 1.000000. mission 간격이며 GPU 표시 시간이나 애니메이션FPS가 아니다.

아래 단위는 **관측 1초당 경과시간 환산 ms**, 8개체 합계다. 중첩 열을 더하지 않는다.

| 구간 | 유효 초 | FOLLOW | 구역 판정 | 연결 선택 | 표면 이동 | Walk | DriveGround |
|---|---:|---:|---:|---:|---:|---:|---:|
| LOW_HOLD | 39.975 | 28.802 | 0.075 | 0.152 | 12.218 | 16.335 | 16.095 |
| HIGH_HOLD | 59.990 | 27.327 | 0.081 | 0.061 | 14.838 | 14.516 | 14.274 |
| GROUND_HOLD | 30.016 | 11.438 | 0.062 | 0.029 | 0.844 | 8.080 | 7.907 |
| GROUND_EAST | 9.949 | 15.115 | 0.080 | 0.000 | 0.000 | 12.443 | 12.193 |
| GROUND_EAST_HOLD | 14.952 | 14.084 | 0.082 | 0.000 | 0.000 | 11.291 | 11.048 |
| GROUND_NORTH | 9.974 | 14.826 | 0.083 | 0.000 | 0.000 | 12.034 | 11.776 |
| FINAL_HOLD | 129.975 | 11.290 | 0.077 | 0.000 | 0.000 | 8.575 | 8.355 |

전체 함수 호출/초·평균 호출 비용과 SearchApproach/SurfaceYield 항목은 공개 JSON/선정 패킷에 포함한다. 개체 포괄·단조 증가·자식 구간의 FOLLOW 상한 검사는 {"completeActorCohorts": true, "monotonicCounters": true, "childrenBoundedByFollow": true}. 전체 표본 집합 14977개. 이번 표본 예산 계약 True.

## 4. 원인 분석

확인: 기존 계측에서 합쳐졌던 FOLLOW 내부 함수 경과값을 실제 실행 로그로 분리할 수 있다. 이8명 실행의 LOW/HIGH_HOLD에서 PlanConnection은 각각 0.152/0.061ms/관측초, DriveGround는 16.095/14.274ms/관측초였다. 연결 선택 하나가 FOLLOW 비용을 지배한다는 설명을 이8명 조건의 수치는 뒷받침하지 않는다. 64명에서는 별도 검증해야 한다. 이는 비용 계측 경로를 확보한 것이며 전체 병목 원인이나 수정 효과의 최종 입증은 아니다.

한계: SurfaceWalk/SearchApproach는 Walk/DriveGround를 포함할 수 있다. 최초 Floor와 인라인 입구 정렬은 FOLLOW의 나머지 범위에 남아 있다. 어떤 열도 순수 경로 계산·CPU self-time·GPU 시간으로 바꿔 부르지 않는다. 타이머·기록의 부하가 포함되며 인과적인 개선량은 아직 없다. 8명 비용을64명 비용으로 곱해 예측하지 않는다.

애니메이션: CPU 포즈와 실제 표시 시간 불일치 원인은 이번에 해결하지 않았다. nativeSeconds는 누적 콜백 dt이며 직접 graph phase가 아니다. 네이티브 그래프 수신자/렌더 제출/GPU 및 할당·GC 구간은 현재 미확보다. 새로운 보간·시간 주입·업데이트 생략은 적용하지 않았다.

## 5. 검증 수준

- 빌드·자동 서명·서명 검증·15스크립트 packed 일치·기존14파일 불변: 통과.
- OFF/ON 정책과 실제씬 엔진 컴파일: 진단 성공. 실제 물리/시각 품질 수용과 구분.
- 추가 함수 타이머의8개체 실제 기록·포괄·중첩 검사: 진단 성공.
- 이번 단일8명 기능 게이트: True. 반복 비용 검증 아님.
- 새 패키지64명 회귀·대조 반복·성능 개선·시각 품질·전체 골: 미완료.20ms p95는 목표다.

## 6. 변경 사항

추가 파일 DGSubsetZZZZZZZFollowCost.c, 격리 candidate-follow-cost-split-v2. 기존 B93D·작업본·운영 서버·원시 로그·개인 키를 변경하지 않았다. 배포 후보에 개인 키 없음. 패키지/런처·정책시험 mission은 실험 폴더에 연결했다. 실제씬 mission은 B93D와 동일한 소스 해시다. 초기 실패 자료를 삭제하거나 성공으로 덮어쓰지 않았다.

## 7. 원시 증거

실행 ID runtime-20261031-081814: launch.json, native-follow-cost.tsv, native-drive-cost.tsv, native-ground.tsv, native-state-contract.tsv, 함수/비용/상태/자극/예산/끝점 분석 JSON. 실험 plan.json/revision-v2.json/policy-fixture-revision.json/packed-and-compile-proof.json/results-eight.json과 각 compile profile을 연결한다. 공개본은 선택된 본문·함수 경과값이며 로컬 경로·키·프로세스 정보·원시 전체·영상은 포함하지 않는다.

## 8. 잔여 문제

8명 기능과 계측 검사를 검토한 뒤 같은 패키지의64명 시험에서 범위·비용을 확인한다. 그 뒤 동일 조건의 순서를 교차한 OFF/ON 반복으로 계측 자체의 영향을 구분한다. 본 기능을 가볍게 바꾸는 최적화는 비용 귀속 전 보류한다. 인라인 지지/입구 정렬의 잔여 비용이 크면 해당 부분만 추가 계측을 설계한다. 애니메이션 시간·실제 표시 진단은 이 비용 실험과 분리해 계속한다.

선정 공개 본문. 로컬 보고서 SHA-256: `d38a11eebd58f20ac06badcc4a9edfc2e01344a40e34b4ef72a1b76519e6dde8`. 원시 전체가 아니다.
