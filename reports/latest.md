# Lazy eligibility — 실제8개체 상태 대조

## 1. 작업 목표
국소 선별을 통과한 lazy eligibility가 실제8개체 추적·등반·낙하·착지 중에도 원래의 이동 가능 판정과 일치하는지 확인한다. 물리 질의·밀침 힘·지속 등반·애니메이션 갱신은 유지한다.

## 2. 실제 수행
기존 13개 PBO와 현재 single render+receiver 구성을 재사용했다. 별도 미션에만 후보 클래스를 넣고 실제 쌍마다 캐시된 canA/canB와 원래 Movable/EntryPressurePriority를 다시 계산해 비교했다. 원래 계산을 추가 실행한 shadow이므로 성능 시험이 아니다. 한 실행에서 고정90게임초의 ground/low/high 목표 이동을 반복했다. 원시 코드·패키지 변경과 신규 PBO는 없다.
준비 중 관측기 예상 수64/실제8 불일치를 발견했다. 새 런타임을 무조건 재실행하지 않고 실제 클라이언트 렌더8과 서버 host-ready8render/8receiver를 교차 확인해 시작했다. 기대 수가 잘못된 관측기는 추가 enumeration을 했으므로 이번 실행 비용을 비교 근거로 쓰지 않는다. 다음 실행 seed에는 기존 지원 nf-cost-logical-eight.flag를 추가했다. 기존 대기 controller는 PID/시작Ticks 대조 후 종료했다.

## 3. 측정 결과
90.0182게임초, Crowd.Update4502회, 원래 판정과 대조57428회, 캐시 계산23620회, 불일치0. 실제 행동2776행. 전원 목표 인식 및2m 이상 이동, 관측된 렌더8/피격 호스트8 생존. 최종 VALID_TERMINAL8/8. 등반42·하강9·착지15 누적 관찰. FOLLOW/MOTION/SETTLE/EGRESS/FALL/LAND 경로가 나타났다.
접촉 솔버32773회, contactFailures395회, 경로 계획204회/실패35회는 원시 카운터다. 정상 후보 거절·복구를 포함하며 이 수치를 terminal 실패 수로 바꾸지 않는다. 기준 실행과의 증감 비교는 미실행이다.
서버 RequestExit(0), 실제 exitCode-1/forced=false, 클라이언트 정상 종료 확인. 컴파일/스크립트 오류·native crash 기록 없음. 전체 CPU, Present p50/p95/p99, 실제 시각적 부드러움은 미측정.

## 4. 원인 분석
확인: 관측된 실제 쌍 처리 중 이동 가능 상태 재사용은 원래 판정과 같았고, 최종 목적·복귀는8/8이었다. 일반적인 지지·충돌 검사 삭제 없이 통과했다.
추정: 반복 상태 조회 감소가 국소 시간 감소의 원인 후보지만 이번 shadow에는 원래 판정도 있어 성능 개선을 입증하지 못한다.
미확인: 별도 baseline과의 기능 차이, 실제 사망·피격 중 모든 분기, 네이티브 callback의 재진입, 64개체 전체 CPU 효과와 시각 품질. 이번 8/8을 모든 기능 최종 수용으로 일반화하지 않는다.

## 5. 검증 수준
실제8개체 판정 경로 진단 성공. 해당90초 시나리오 종결8/8. 기준/후보 기능 회귀 비교·독립 반복·시각 품질·성능·최종 수용은 미완료. 물리 검사나 애니메이션 갱신율을 줄인 시험이 아니다.

## 6. 변경 사항
LAZY-ELIGIBILITY-eight-shadow1, 별도 mission/init.c의 후보 클래스와 연결만 변경. 기존게임/PBO/자산/운영 서버 보존. 신규 PBO가 없어 새 서명 대상 없음. 준비 계약·미션 해시·13개 입력 해시는 contract.json으로 연결한다. 관측기 기대 수를 고치는 seed flag 추가는 이후 실행용이다.

## 7. 원시 증거
contract-preparation.json/contract.json, init.c, start-receipt.json, gate-launch.json, server/client launch·terminal, gameplay-eight.tsv, mixed-hosts.tsv, terminal-state.tsv, milestones.tsv, part-command-clock.tsv, server-frame.tsv, script 로그, analysis.json. 실행 ID LAZY-ELIGIBILITY-eight-shadow1. 공개 패킷은 자체 후보 클래스와 선정 요약·원시 파일의 hash만 포함하고 로그인/플레이어 식별자·내부 경로·전체 원시 로그는 노출하지 않는다.

## 8. 잔여 문제
다음 단일 비교는 shadow를 끈 동일 진단 구성의 기준/후보 전체 CPU 비교다. 8개체에서는 군집 규모가 작아 전체 이득이 작을 수 있으므로8/8을64성능 성공으로 환산하지 않는다. 64개체에서는 동일 조건 대조와 반복으로 전체 CPU 개선 여부를 판정하고 정상 밀침·낙하·재등반을 계속 보존한다. 바닐라 내부 Ray/Sweep 빈도와 접촉 재사용은 여전히 미확인. 20ms p95는 미달성 목표다.
