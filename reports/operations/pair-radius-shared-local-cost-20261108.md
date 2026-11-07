# 군집 접촉 반경·거리 공유: 국소 비용 반복 통과

## 1. 작업 목표
비접촉 쌍의 불필요한 상태/속도 계산을 줄이고 실제 군집 Update 전체 비용과 결과 동등성을 비교.

## 2. 실제 수행
첫 중복 거리 버전은108행·10800Update 시험에서 완전겹침 +6.58%로3%회귀 기준 실패, 미수용.
별도 수정안은 원래 ContactForce의 순수 산술을 pair 처리로 옮겨 away/gap을 한 번만 계산해 재사용. 임계값·힘·이웃/누적순서·물리 검사는 보존.
원본/sham/후보를8조건×6균형순서×100Update로 비교. 동일 미션 바이트를 새 프로세스로 반복:144행·14400Update씩, 독립 프로세스2개.

## 3. 측정 결과
후보/원본의 순서별 상대변화 중앙값:
|조건|실행1|실행2|
|---|---:|---:|
|dense|-28.365%|-28.469%|
|mixed|-15.234%|-15.145%|
|coincident|-9.420%|-9.958%|
|dead|-25.919%|-24.058%|
|LAND|-13.473%|-11.719%|
|sparse|-0.289%|-1.095%|
|boundary|-0.529%|-0.155%|
|vertical|+1.616%|+2.463%|

두 실행 모두 exact 결과 불일치0. dense/mixed5%이점 및 제외조건3%이내 회귀 기준 통과.
TickCount 기반 메서드 elapsed 비율이다. self-time·전체CPU·Present ms가 아니다. 논리64, native entity0. 실제8/64기능·시각·p50/p95/p99미측정.

## 4. 원인 분석
원본은 반경 밖에서도 상태와 상대속도를 계산했다. 가까운 쌍의 중복 거리 계산을 없앤 버전에서 겹침 회귀도 사라졌다.
inline 호출 감소와 조기 반환의 개별 기여는 분리하지 않았다. 국소28%를 전체CPU28%로 확대할 수 없다.

## 5. 검증 수준
LOCAL_METHOD_GATE_REPEATED_PASS_NOT_WHOLE_CPU_ACCEPTANCE.
국소 동등성·비용 독립 프로세스2개 반복 통과. 실제 기능·시각·전체 성능·배포 최종수용 미완료.20ms 달성 아님.

## 6. 변경 사항
격리 미션·계약·분석만 추가. 기존 게임소스/운영PBO 변경 없음, 새 서명 대상 없음.
선택 버전 mission SHA256 f52d509b73daecc0e99261731240b2d9188c08257642317b03316fdc039cd580.

## 7. 원시 증거
ANIM-pair-radius-method-cost1(미수용), ANIM-pair-radius-shared-method-cost2, ANIM-pair-radius-shared-method-cost3-repeat.
내부 pair-cost.tsv·plan·로그·종료·analysis·local-gate-summary 보존 및 저장 전 해시 검증. 서버 요청 종료 뒤 actual exit-1, 강제 종료 없음. 민감 원시 파일은 공개하지 않음.

## 8. 잔여 문제
Combat Active/PendingDeath·managedCLIMB상태 보완, default-OFF 서명 격리 binding, bounded8 통합과64전체CPU선별이 남음.
기존8입구정체는 별도 기준결함으로 유지. 기존 실패를 반복하는 장시간 기능 개발로 확대하지 않음. 실제 정상 접촉·낙하·착지·피격/사망·시각 최종검증은 남아있음.
