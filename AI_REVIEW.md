# DayZ NPC Observatory — 현재 상태

2026-11-09: **현재 자동명령 경로의 로컬 파트 capability 시험은 정지 포즈로 기각됐다.** 기존 서버5파트를 유지하고 클라이언트에 같은 원본파트1개를 추가했다. 유효1000표본 모두 appActive=true, 기준파트998/999전환에서3본중하나이상변화, 로컬3본변화0·애니메이션/물리콜백0·Preview요청2/Native요청0이었다. 단순로컬생성과기존요청설정만으로현재재생경로가성립하지않았다.

- [상세 결과·수집기 실패·제한](reports/operations/client-local-role-capability-20261109.md)
- [구조화 결과](data/client-local-role-capability-20261109.json)
- [컴파일ANM 기존키축약](reports/operations/anm-compiled-counts-review-20261109.md)
- [1mm 후보 비용 선별 실패](reports/operations/foot-range-tolerance1-method-20261109.md)
- [기존 보고서](reports/operations/)

전체클라이언트로컬애니메이션불가능판정은아니다. 명시적명령초기화는미시험이다. observer Age0필드는타이밍분석에서제외했고첫수집실패도보존했다. CPU/실제표시품질/8→64수용미측정. 원본PBO불변, 진단PBO2개서명검증성공.20ms p95미달성.

시작위상이정렬되지않아동시포즈동등성을비교하지않았다. 자동생성경로기각을모든지원API/엔진기능불가능으로확대하지않는다. sourceAge0원인미특정, 표시부드러움이나성능이득주장없음.
