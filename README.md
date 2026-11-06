# DayZ NPC 현재 상태

재사용 외에 실제 접촉/후속질의 발생을 줄일 후보를 검토했다. 기존3입구 분산은 이미 작동한다. low레인0.8m/contact기준0.86m의차이를확인했고 0.8~1.05m 정적지지18조건은통과했다. 모션·CPU효과는미검증.

[상세](reports/operations/work-reduction-lane-feasibility-20261107.md) · [JSON](api/public/work-reduction-lane-feasibility.json) · [함수·근거 TXT](evidence/work-reduction-lane-feasibility-20261107.txt)

다음후보는low레인만0.95m로맞추는기본OFF격리안과호출수계측명세. 현재게임코드/PBO미변경. 셀풀새엔진첫OFF자료와남은5회는보존·미완료. 전체20ms·64기능·시각최종수용미완료. 골active.
