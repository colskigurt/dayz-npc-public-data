# 4파트8명 기능 대조 검토

4파트 후보와5파트 대조군 모두8/8 전체 이력·최종 도달,낙하9/9복귀·상태 계약 오류0. 각1회이며64명·시각·전투·CPU/Present 비용은 미검증이다.

[상세](reports/operations/four-part-eight-functional-20261106.md) · [데이터](api/public/four-part-eight-functional.json) · [어댑터 변경과 대표 원시 행](evidence/four-part-eight-functional-20261106.txt)

4파트 flag 기본OFF,5파트 flag와 충돌 시 거부. 변경 역정규화는 기존 어댑터와 일치. 물리 질의·피해 수신체·충돌 차단·본 sentinel·퇴역·추적·낙하 코드 유지. 직접 클립 검증 그래프 대신 기존 native_traversal 사용.

각 조건1회이고 후보를 먼저 실행했다.4파트 완료시간80.8초,5파트58.9초: 접근 지연 차이의 재현성과 인과는 미확인이다. 비용 개선으로 보고하지 않는다. combat=false이므로 피격·전리품·공격은 미검증이다.

다음 단일 작업은 기존64명 시나리오의4파트 검증. 실패 시5파트 기준 재현을 먼저 확인하고 최적화 전체를 기능 수정으로 전환하지 않는다. [직전 안내](reports/history/20261106-before-four-eight-AI_REVIEW.md)
