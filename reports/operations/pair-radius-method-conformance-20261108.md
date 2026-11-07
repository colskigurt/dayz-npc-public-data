# 접촉 반경 조기 판정: 메서드 동등성

## 1. 작업 목표
기존 군집 AccumulatePair의 수평 접촉 반경 판정을 앞당겨도 원본 계산 결과를 보존하는지 확인. 성능 개선은 별도 판정.

## 2. 실제 수행
기존 grid-cell-pool method4 하네스 재사용. 원본180입력에 경계/완전 겹침64입력을 추가하여 현재 retail Enforce 서버에서244프레임 실행. 미션 subclass만 격리. 원본 사망/높이 필터 뒤에 동일한 수평 Length >=0.86 반환을 추가하고, 반경 안 힘 계산과 이웃/누적 순서를 유지.

## 3. 측정 결과
244행에서 힘·속도·proposal·entry priority·pair/contact 수 exact mismatch0. 논리64개체, native entity0. 서버 RequestExit 후 actual exit-1, 강제 종료 없음.
CPU p50/p95/p99, Present, 실제 화면, live8/64 기능 회귀는 미측정.

## 4. 원인 분석
후단에서 zero 처리하는 반경 밖 쌍을 먼저 반환해도 해당 입력 결과는 같았다. 반복 Movable·상대속도 작업 감소는 후보 원리이며 CPU 효과는 미입증. 반경 안 쌍의 거리 계산 중복으로 느려질 수 있다.
이전 스냅샷114개에서 높이 통과71441쌍 중38372쌍이 반경 밖이었으나 실제 갱신의 생략률/성능 절감률은 아니다.

## 5. 검증 수준
METHOD_CONFORMANCE_PASSED_ONLY. 반복 성능·실제 기능·시각·최종 수용 아님.20ms 목표 달성 주장 없음.

## 6. 변경 사항
격리 미션과 계약만 추가. 기존11PBO 해시 확인, 게임 소스/배포 PBO 변경 없음. 새 서명 대상 없음.
mission SHA256 4fcf22fb69db1fd6d31d638b68ceaabe7d7b470ec6290eaba9d9fb62a15f1075

## 7. 원시 증거
실행 ID ANIM-pair-radius-method1.
내부 보존: grid-method.tsv SHA256 e7172c4ba41c1d26ea5989811b75d68062d4ef7512bf999dda77aaae8d23e0ed, source-and-contract, plan, 서버 로그/종료 기록, analysis. 원시 자료·로컬 경로·프로세스 정보는 공개하지 않음.

## 8. 잔여 문제
Combat Active/PendingDeath 매트릭스 및 native 이동·밀침·낙하 미검증. 다음은 원본/sham/후보 순서 균형의 실제 군집 Update 국소 비용 비교. 5%국소 이점과 제외조건3%이내 기준 미통과 시 live8/64 확대하지 않음. 기존 기능 결함은 별도 관리.
