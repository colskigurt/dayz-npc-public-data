# DayZ NPC Observatory — 현재 상태

2026-11-09: **하네스 제어문 범위 문제를 재현·수정하고64→248 골격 목록 쌍의 CPU 포즈 비교를 완료했다.** entity0 시험에서 false 조건의 무중괄호 버전 후속 증가0/중괄호 버전1. 범위 명시 후 준비·표본 제한·정상 퇴역이 회복됐다.

- [최신 상세 보고서](reports/operations/cardinality64-braced-20261109.md)
- [구조화 결과](data/cardinality64-braced-20261109.json)
- [앞선 관측·퇴역 실패 보존](reports/operations/cardinality64-pair-20261109.md)
- [정적 동등·단독 생성](reports/operations/cardinality64-factory-20261109.md)
- [기존 보고서](reports/operations/)

서버4,800행/클라이언트4,288행, 공통64본 최대 모델 공간 위치 오차0m. 클라이언트 활성4,288/4,288행. 서버 detached=true·강제 종료 없음, 클라이언트 정상 종료. 신규 관측PBO 서명 검증 성공, 기존 NPC·물리·그래프 보존.

**전체 가중 본 통합·프레임별 실제 화면 부드러움·8/64 기능·CPU 절감·20ms p95 목표는 미해결.** 이번 모델은 여전히64개 본만 가중한다. 다음은 과거 fullbody 실험과 중복을 확인한 실제 가중 본 확대 입력 계약이다. 과거 시험 전체를 이번 하네스 결함만으로 소급 무효화하지 않는다.
