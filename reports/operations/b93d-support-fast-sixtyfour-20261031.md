# B93D 지지 검사 단축 후보:64개체 기능 회귀와 정상 낙하 복구

작성: 2026-10-31T14:39:25.978338+09:00

## 1. 작업 목표

B93D의 기존 제한적 SupportSweepFast 옵션을 8명에서64명으로 확장해 기능 회귀를 확인한다. 좁은 지붕의 전원 동시 점유는 요구하지 않는다. 정상 밀침·낙하·착지·재추적은 보존하고 유효 최종 상태·복구 계약·미해결 알고리즘 실패를 구분한다.

## 2. 실제 수행

앞선 shadow 적격 비교와 전경8명 활성 gate를 확인한 뒤, 같은 미션/PBO/기존 상세 계측으로64명300초를 실행했다. 플레이어는 물리적으로 자유롭고 고정 목표 장치는 끈 상태다. 고정 카메라를 사용하고 컴퓨터 유즈로 로딩·장면을 확인했다. 실제 전경 비율은 로그로 판정했다. 기존3PBO 서명 검증 성공. 새 PBO·소스 알고리즘·native 후킹·시계 보정·녹화는 없다. 정상 종료 뒤 기존 상태 계약 분석기와v3 지지 분석기를 사용했다.

## 3. 측정 결과

실행 runtime-20261031-143044, 64명300초, 전경 비율 1.0000. 최종 유효 ARRIVED 63/64, 낙하 후 착지·FOLLOW 복귀 95/96, 등반 종료 실패 0, 실제 알고리즘 실패 라벨 ['ENTRY_ALIGNMENT_NO_PROGRESS', 'EGRESS_STALL']. 같은 호출의 물리 전이 연결 True.

낮은/높은 면 FOLLOW를 한 번 이상 관측한 개체 64/63명이다. 동시 점유 수 또는 모든 개체가 같은 시간에 등반했다는 뜻은 아니다. 정상 낙하 횟수를 줄여 성공을 만들지 않는다.

실제 bound 사용 270046회, 생략 광선 1080184회. 기본 ray+Fast sweep/solve 8.2526. 모든 physics 질의 수가 아니다. fast-mode referenceChecks=0이므로 under=0을 새 정확성 증거로 사용하지 않는다. 앞선 적격 shadow 비교56,648회·과소0·최대과대4.364mm와 구분한다.

전경 mission frame p50/p95/p99 24.0/65.0/86.0ms. 실제 표시 포즈 cadence/GPU Present가 아니다. 단일 실행이며 이전 자유/고정 목표 OFF의 frame 수치와 직접 차감해 성능 개선율을 주장하지 않는다.

gate:
- all64ValidFinalStates: False
- allFallsResumed: False
- allPhysicalTransitionsMatched: True
- noClimbEndFailure: True
- noAlgorithmFailureLabels: False
- foregroundCollected: True
- actualTargetStagesValid: True
- activeBoundActuallyUsed: True

이번 범위 기능 판정: False. 실제 목표 단계 검사는 {'LOW_HOLD': True, 'HIGH_HOLD': True, 'GROUND_HOLD': True, 'FINAL_HOLD': True}다. 표본 사이 연속 지지나 모든 일반 맵·전투 상황까지 증명하지 않는다.

## 4. 원인 분석

SupportFast는 매번 중앙 ray와 엄격한 구조물 guard를 확인한 뒤 적격인 경우만 주변4ray를 대체한다. 월드/관절 sweep·지형·미확인 표면·낙하/착지 조건은 보존됐다. 옵션이실제로 작동했다는 counter와 기능 결과를 구분한다. 개체별 접촉·기하·시계가 변하므로 한 번의 질의 감소를 전체 성능 인과로 대체하지 않는다.

미해결 gate가 있으면 raw와 미도착 개체를 보존하고 해당 상태·generation·접촉 문맥을 조사한다. 정상적인 지지 상실 후 복귀는 실패가 아니지만 EGRESS_STALL·CLIMB_EXIT_VALIDATION·미복귀를 숨기지 않는다. 높은 면의 전원 동시 점유를 성공 기준으로 추가하지 않는다.

이번 미복귀는5번 generation1이다. 66.032초 CONTACT_SUPPORT_LOST로 FALL에 들어간 뒤 종료까지 LAND/FOLLOW가 없다. 80.032초 표본에서 분리 이동 누계6.00022m/한도6m, 이후 SEPARATION_DISTANCE_LIMIT가 반복된다. 마지막299.197초 착지 검사는 FIXTURE_VOLUME 거부다. root의 ground 영역 분류는 착지 완료가 아니다. 100.182~299.648초 Fast 추가 사용0, 접촉 solve121회·발 광선1210개로 기존10ray/solve가 실행됐다. 이 구간의 거부는 현재 착지/분리 guard에 연결되지만, 초기 낙하 전 Fast의 경로 의존적 영향까지 배제한 인과 증거는 아니다.

35번은53.429초 ENTRY_ALIGNMENT_NO_PROGRESS에 따른 FALL 후 최종 ARRIVED,12번은71.433초 EGRESS_STALL 후 최종 ARRIVED였다. 나중 도착했다고 두 계약 실패를 지우지 않는다. 이들은 정상 압력 낙하95건의 복귀 성과와 별도로 조사한다.

## 5. 검증 수준

실행 종료/서명/실제 옵션/계측 진단은 성공. 현64명 단일 실행 기능 gate는 False이며 반복 검증 완료·20ms 성능 목표·시각 품질·전체 최종 수용은 아니다. 애니메이션 CPU/실제 화면의 시간 불일치와 지원되는 수정 경로는 별도 미해결이다.

## 6. 변경 사항

B93D B93D2E713FE7AEB41570C2B21839FF1ADA148BE0EAC090747220B6A4F78AD0D1와 기존453D/0C6E 자산을 유지했다. 새 PBO·재서명 없음; 기존3개 서명 검증 성공. 기존8명 보고서·v1/v2/v3 분석 이력·원시 기록·운영 서버·개인키를 보존했다. 이번엔64명 gate 분류·보고·공개 파생물만 추가했다. 무분별한 캐시·world 검사 삭제·새 animation frame skip·Simulation LOD는 적용하지 않았다.

## 7. 원시 증거

fast64 launch/native-ground/native-natural-player/native-state-contract/native-traversal-events/state-contract-report/pose-cadence와 source-audit/plan/분석/64admission을 연결한다. 앞선8명 checkpoint를 함께 참조한다. 저장 전 해시·JSON·파일 안정성·게임/기록 종료를 확인한다. 공개 자료는 집계·선정 소스와 미도착 문맥만이며 전체raw·실행 포인터·프로세스·로컬 경로·키·자산·영상은 제외한다.

## 8. 잔여 문제

현재 gate는 실패했으므로 성능 배치는 진행하지 않는다. 다음은5번의 기록된 root/heading/lift/4joint를 기존 격리 접촉·탈출 replay에 입력해 착지 체적 거부와 분리 한도 소비를 재현하는 일이다. timeout/거리 한도만 늘리거나 world 검사를 삭제하지 않는다. 35번의 진입 watchdog과12번의 출구 handoff 실패도 원시 문맥을 보존해 확인한다. 이후 기능 gate통과 시에만 같은 현재 미션/PBO·고정 목표·동일 camera/graphics/계측의 OFF/FAST/FAST/OFF 비교를 한다. 기존 애니메이션 시계·실제 표시 연구는 대체하지 않는다. 20ms p95는 계속 목표다.

선정 공개 본문. 로컬 보고서 SHA-256: `7ff629635d4152681702646e65fa03da9151ce77ae6a5ad296640a6ac98950e6`. 전체 원시 증거가 아니다.
