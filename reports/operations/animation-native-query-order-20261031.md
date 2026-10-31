# 애니메이션 native 호출과 CPU 조회의 직접 QPC 순서

작성: 2026-10-31T16:42:56.052769+09:00

## 1. 작업 목표

기존 잔여시간 보존 진단의 평균 속도 복원과 최종 표시 끊김을 구분한다. 인접 script frame 상관관계를 실제 native QPC 순서와 대조하고, CPU 조회까지의 지연을 측정한다. 새 보간·frame skip·배속 수정은 적용하지 않는다.

## 2. 실제 수행

기존 A/B/B/A 네 실행의 residual_entry, 독립 clock bridge, visible-frame-clock, source pose fit을 재분석했다. 각 fit의 nativeSequence를 정확한 event에 연결하고 이전 CPU callback 종료≤native read≤다음 callback 시작을 검증했다. source fit을 그래프 시계에 맞춰 재선택하지 않았다. 기존 입력 hash를 다시 대조했다. native clock+submitted numerator를 동일 QPC에 배치하고 평균 기울기/상수 offset을 회귀로 제거한 잔차를 비교했다. 이 detrending은 분석이며 실행 보정이 아니다. 새 runtime·메모리 쓰기·PBO·영상은 없다.

## 3. 측정 결과

| 기존 실행/조건 | 표본 | native 간격과 단계 상관 | native→다음 CPU 조회 p50/p95/p99 ms | query 시점 잔차 RMS ms | 이전 query 시점 잔차 RMS ms |
|---|---:|---:|---|---:|---:|
| runtime-20261031-110449/baseline30 | 1472 | 0.493362 | 19.062/33.504/34.996 | 71.103 | 70.739 |
| runtime-20261031-111204/candidate30 | 1471 | 0.992817 | 18.881/33.189/34.732 | 12.601 | 0.455 |
| runtime-20261031-112434/candidate30 | 1472 | 0.992338 | 18.358/33.240/34.898 | 12.617 | 0.440 |
| runtime-20261031-112900/baseline30 | 1473 | 0.527604 | 19.385/34.113/36.007 | 81.380 | 81.015 |

후보 두 실행에서 유효 단계와 현재 native-entry 간격 상관은0.99282/0.99234, 차이RMS1.895/1.891ms다. 이전 native-entry 간격과는 음의 상관이므로 단순히 모든 자료를 한 native frame 옮기는 설명은 맞지 않는다. 이전 CPU-query 간격과는0.99925/0.99918, 차이RMS0.491/0.514ms다. CPU query 시점에서 회귀 잔차RMS는12.601/12.617ms지만 이전 query 시점에선0.455/0.440ms, native-entry 시점에선1.256/1.253ms다.

기본 두 실행 native-entry 잔차RMS70.746/81.009ms는 평균 속도 차를 회귀로 제거한 뒤에도 남는다. 후보와 기본의 native→다음 query 지연 분포는 비슷한 넓이를 갖는다. 이 지연은 engine interpolation 함수 실행시간이나 GPU latency가 아니다. 모든 선택 event는 CPU 조회 바깥에 있고, 다음 callback까지의 대기 구간을 측정한 것이다.

실제 frame p50/p95/p99·64기능·GPU/Present·새 시각 품질 시험은 없다. 표의 p50/p95/p99는 native→query 지연 분포다. 기존1개체8native파트 조건을8개체로 세지 않는다.

## 4. 원인 분석

확인: 잔여시간 보존 후보는 실제 native QPC 진행과 강하게 일치한다. 이전 script 간격과의 높은 일치는 native 호출이 이전 query 뒤/다음 query 전에 있기 때문에, script 표본번호와 native 표본번호의 단계 차를 포함한다. 후보의 CPU query 시점에서 보이는 약12.6ms 잔차를 그대로 native clock 자체의 불규칙성으로 귀속할 수 없다. 다른 시간 기준을 썼을 때 잔차가 다르게 보인다는 구체적 근거다.

미확인: 이 결과가 render buffer 한 프레임 지연·saved dt 소유권·GPU pose 소비 순서를 유일하게 증명하지 않는다. 네이티브 entry보다 늦게 queried pose가 보였다는 순서와 실제 GPU 소비는 다르다. CPU 조회 bracket은HUD/로그 준비·여섯 getter를 포함하므로 각 getter의 정확한 시각도 아니다. VSync·계측·녹화 scheduling 영향은 아직 분리하지 않았다. 기존 영상의 silhouette/HUD 연계는 보존하되 이번에 새 decoding을 하거나 smoothness를 수용하지 않는다.

반증 범위: 후보가 평균 속도만 맞고 native-entry 시간에서도 같은 정도로 심하게 흔들린다는 해석은 이 표본에 맞지 않는다. 반대로 현재 query interval과 높은 상관이 없으므로 후보가 잘못됐다는 해석도 시간 순서 확인 없이 성립하지 않는다. 이 진단을 제품용 native memory patch 승인으로 확대하지 않는다.

## 5. 검증 수준

기존 입력 무결성·event/CPU bracket 순서·direct-QPC 재분석:진단 성공. 기존 네 실행을 재사용했으며 새4회 반복이 아니다. 지원 배포 경로·최종 표시 품질·64비용·기능 회귀·20ms p95·최종 수용:미완료.

## 6. 변경 사항

분석 스크립트·결과·보고 파생물만 추가했다. 기존Source·미션·PBO·운영 서버·원시로그·영상·키는 변경하지 않았다. 기존B8FB 빌드의 격리 진단을 읽었으며 새 서명 대상은 없다.

## 7. 원시 증거

네 실행 launch·native calls·visible-frame-clock·clock bridge·기존analysis/pose-window를 연결한다. 원시 event와 quarter-key source pose는 서로 다른 측정/추정 층이다. 파생 패킷은분포·선정 처음/마지막 pairs이며 전체raw가 아니다. 저장 전 기존 hash 대조와 안전 종료를 확인한다.

## 8. 잔여 문제

다음은 실제 native dt 저장·평가·보간 소비 시점과 queried pose의 연결을 좁히는 일이다. 현재 자료의지연을보정하는새보간을먼저적용하지않는다. 필요시 기존 하네스에서 최소한의 비변경 관찰자로 입력dt/조회 순서를 측정하되 실제 GPU/Present 접근 가능성과 계측부담을 먼저 확인한다. 지원되는 asset/API 경로 확보와64비용은 별도 gate다. 정상낙하·착지·FOLLOW·재등반과 반복LOW 자극 불일치 판정은 변경하지 않는다.

선정 공개 본문.로컬 보고서 SHA-256:`f1de17aac3d303f87ba710227c4c54f2901bf8ee303c079cbfd1f57139f624b6`.전체 원시 자료가 아니다.
