# 64개체 실제 함수 소유자 비교

## 1. 작업 목표
8→64개체의 실제 스크립트 호출 소유자를 비교하고, 기존 기각 후보를 반복하지 않는 국소 비용 절감 후보를 선정한다.
## 2. 실제 수행
같은 진단 서버·클라이언트, 게임 PBO 11개, 5표시 파트+1피해 수신체 구조로 30초 준비 후 30.001게임초를 수집했다. 첫 runtime1은 프로필 복사 중 필수 traversal manifest 누락으로 스폰 전 실패했다. 원시 실패·강제 서버 정리 기록을 보존하고 동일 manifest를 복사한 runtime2만 진단에 사용했다. 알고리즘·PBO·기존 실행기·보안 설정은 변경하지 않았다.
## 3. 측정 결과
64/64 목표 인식·측정 구간 이동, 320표시+64수신체 유지. 진단 717프레임. SolveContactsInternal 70704회(개체당 초당36.824), 8개체는5941회(24.744). Drive37056회(19.299/개체초), 8개체4800회(19.991/개체초). Crowd.Update718회, 8개체1500회. CellKey459520회, AccumulatePair409250회. 일반 클라이언트 p50/p95/p99는 이번에 미측정이다. 진단 프레임 빈도를 retail 성능으로 해석하지 않는다.
## 4. 원인 분석
밀집 조건에서 개체당 접촉 솔버 호출 증가를 확인했다. 호출 순위에는 Drive→Traversal.Follow/Walk 및 Crowd.ConsumeContact/Displace가 나타난다. 정적 단위와 집계 의미가 미확정인 rawTime은 중첩 합산·ms 변환·CPU 점유율 환산 금지다. CellKey는 원시시간 순위52위로, 호출수만으로 주력 최적화로 선정하지 않는다. 신규 검토 후보는 TrySupportBypass의 호출 내 동일 진입 SupportStepClear 재사용이다. 24후보는8방향×3거리이며 entryStep/entryHeading은 거리에 무관하다. 함수 내 지역 캐시로 최대16회 중복을 피할 수 있는 구조적 가능성이 있으나 실제 적중수·전체 절감률은 미측정이다. 기존 Drive→Displace 입력 불일치 캐시, SupportSweepFast, 셀 풀링과 범위가 다르다. 검색한 기존 기록에서 같은 후보는 확인하지 못했으며 전체 이력 미실행을 단정하지 않는다.
## 5. 검증 수준
함수명·호출수 진단 성공, 크기별 각1회. 64개체 전체 기능 회귀·반복 성능·시각 최종수용은 미완료. 측정 중 스크립트 오류 행0. 종료 시 HumanCommandDamage/BunkerBroadcastManager/PGFilter 누수 경고는 보존하며 해결 주장 없음.
## 6. 변경 사항
격리 Count64 미션, 입력 프로필, 분석기와 결과만 추가. 게임 PBO 불변이므로 신규 서명 대상 없음. 서버 정상 종료 요청 후 종료(-1), 클라이언트 정상 창 닫기 확인. runtime1 실패 서버는 명시적 강제 정리였으며 성공 실행의 종료와 구분한다.
## 7. 원시 증거
ANIM-npc-owner-diag-sixtyfour2, 비교 ANIM-npc-owner-diag-eight1. runtime2/analysis.json, qualification-audit.json, server-profile/npc-function-times/counts/selected-owners.txt, gameplay-eight.tsv, mixed-hosts.tsv. runtime1/failure-analysis.json과 failed-run-cleanup.json도 보존. 원시 해시는 아래 검증 기록에 연결한다.
## 8. 잔여 문제
다음 단일 산출물은 호출 내 진입 검사 재사용의 격리 동등성·비용 판정이다. 방향별 입력 동일성, 검사 순서와 결과·경로 선택 동일성, 함수 내 세계 변경 여부를 먼저 확인한다. 사망·낙하·지지·충돌 검사는 유지한다. 기존 하네스로 국소 효과가 반복 확인될 때만8→64 전체 CPU 비교로 확장한다. 정적 최대16회 생략 가능성을 성능 개선률로 보고하지 않는다. 새 후보 구현은 아직 하지 않았다.
